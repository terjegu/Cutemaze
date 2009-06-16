/* ----------------------------------------------------------- */
/*           _ ___   	     ___                               */
/*          |_| | |_/	  |_| | |_/    SPEECH                  */
/*          | | | | \  +  | | | | \    RECOGNITION             */
/*          =========	  =========    SOFTWARE                */
/*                                                             */
/* ================> ATK COMPATIBLE VERSION <================= */
/*                                                             */
/* ----------------------------------------------------------- */
/* developed at:                                               */
/*                                                             */
/*      Machine Intelligence Laboratory (Speech Group)         */
/*      Cambridge University Engineering Department            */
/*      http://mi.eng.cam.ac.uk/                               */
/*                                                             */
/*      Entropic Cambridge Research Laboratory                 */
/*      (now part of Microsoft)                                */
/*                                                             */
/* ----------------------------------------------------------- */
/*         Copyright: Microsoft Corporation                    */
/*          1995-2000 Redmond, Washington USA                  */
/*                    http://www.microsoft.com                 */
/*                                                             */
/*          2001-2007 Cambridge University                     */
/*                    Engineering Department                   */
/*                                                             */
/*   Use of this software is governed by a License Agreement   */
/*    ** See the file License for the Conditions of Use  **    */
/*    **     This banner notice must not be removed      **    */
/*                                                             */
/* ----------------------------------------------------------- */
/*           File: HAudio.c -    Audio Input/Output            */
/* ----------------------------------------------------------- */

char *haudio_version = "!HVER!HAudio: 1.6.0 [SJY 01/06/07]";

#include "HShell.h"        /* HTK Libraries */
#include "HMem.h"
#include "HMath.h"
#include "HWave.h"
#include "HAudio.h"
#include "HThreads.h"
#include "HGraf.h"

static int trace = 0;
#define T_TOP  0001     /* Top Level tracing */
#define T_AUD  0002     /* Trace audio input */
#define T_OUT  0004     /* Trace audio output */
#define T_ECH  0010     /* Trace echo cancellation */
#define T_DMP  0020     /* dump both channels of echo buffer to file */
#define T_SNR  0040     /* output energy in each input block */

/* -------------------- Configuration Parameters --------------------- */

static ConfParam *cParm[MAXGLOBS];       /* config parameters */
static int numParm = 0;
static FILE *dumpf;
static Boolean echoCancelling = FALSE;   /* set true to enable echo cancelling */
static int echoFilterSize = 512;         /* num filter taps 0 .. N-1 */
static int echoMaxDelay   = 1000;        /* max delay in samples */
static int echoFilterAWin =  8;          /* num blocks in filter analysis window */
static float blockEnergy;                /* used by T_SNR option */
static long blockSamples;


typedef enum { ADS_INIT, ADS_OPEN, ADS_SAMPLING,
               ADS_STOPPED, ADS_CLOSED } AudioDevStatus;

#ifdef UNIX

#ifdef LAPTOP_AUDIO
#define PLAYBACKPACKETS 16
#else
#define PLAYBACKPACKETS 8
#endif

#define CALLBACKSPERPACKET 4

typedef struct alsabuf{
   int n;                 /* Number of valid samples in buffer */
  int id;
   Boolean isLast;        /* used on output to signal end of utterance */
   short waveData[WAVEPACKETSIZE];        /* Data in buffer */
   struct alsabuf *next;
} alsaBuf;
#endif

#ifdef UNIX
#define HAudioLock(); pthread_mutex_lock(&(a->mux));
#define HAudioUnlock();  pthread_mutex_unlock(&(a->mux));
#endif

#define ECHOBUFFERSIZE  100   /* num wave pkts in echo ring buffer */
#define ECHOBUFFERHEAD    3   /* num unfilled advance blocks in ring */
#define ECHOTHRESHOLD 2.0E8   /* min energy in 1st blok sufficient to cause echo */

typedef struct {
   short spin[WAVEPACKETSIZE];
   short sout[WAVEPACKETSIZE];
   int tin;     /* approx time of last sample in spin in msecs */
   int oseq;    /* seq number for sout 1,2,3, ... */
}WavBlok;

typedef struct {
   WavBlok buf[ECHOBUFFERSIZE];
   int inx,outx,used;   /* bounded buffer indices and count */
   int lidx;            /* index of last output block */
   int lseq;            /* seq number of last output block */
   Vector fir;          /* the FIR filter 1..echoFilterSize*/
   Boolean firSet;      /* set when FIR filter is set */
   int delay;           /* estimated delay between output and echo */
}EchoBuffer;

typedef struct _AudioIn {
   /* -- Machine Independent Part -- */
   HTime sampPeriod;         /* sampling period in 100ns units */
   AudioDevStatus devStat;   /* indicates when device active */
   int curVol;               /* Current pk-pk volume of input speech */
   EchoBuffer echo;          /* Input ring buf for echo cancellation */
   int updateFilterIndex;    /* set to index of analysis window when update needed */
   /* -- Machine Dependent Part -- */
#ifdef UNIX
  //snd_pcm_t *pcm_handle;
  char *pcm_name;
  pthread_mutex_t mux;
  pthread_cond_t cond;
#endif
}AudioInRec;

typedef struct _AudioOut {
   /* -- Machine Independent Part -- */
   float vol;                /* current volume */
   Boolean isActive;         /* true when output device active */
   AudioIn ain;             /* associated input buffer if any */
   int total;               /* Total number of samples queued */
   int nBlocks;             /* Total number of blocks queued */
   /* -- Machine Dependent Part -- */

#ifdef UNIX
  MemHeap *mem;
  //snd_pcm_t *pcm_handle;
  char *pcm_name;
  alsaBuf *playingHead, *playingTail, *toPlayHead, *toPlayTail;
  pthread_mutex_t mux;
  pthread_cond_t cond;
  HThread callThreadId;
  long int played; /* current buffer pos */
  long int stored;
  long vmin, vmax;
  //snd_mixer_elem_t *mixer_elem;
#endif
}AudioOutRec;

int FakeSilenceSample()
{
   float x = RandomValue();
   if (x>0.8) return 1;
   if (x<0.2) return -1;
   return 0;
}

static int OutSamples(AudioOut a)
{
   int total;
   if (!a->isActive) return 0;
#ifdef WIN32
   if (a->current==NULL) return(0);
   a->mmError=waveOutGetPosition( a->waveOut, a->wavePos,sizeof(MMTIME));
   total = a->total - a->wavePos->u.sample;
   if(a->mmError != MMSYSERR_NOERROR)
      HError(6006,"OutSamples: Cannot get current play back position");
   return(total);
#endif
#ifdef UNIX
   /* TODO: Check correct get frames*/
   {
     int avail, total;
     //snd_pcm_sframes_t delay;
     /* It's positive and less than buffer size in normal situation
	and negative on playback underrun  */
     avail=0;
     if(avail<0)
       HError(6006, "OutSamples: underflow reported");

     total=a->total-avail;
     return total;
   }
#endif
}

int SamplesToPlay(AudioOut a)
{
   if (a==NULL) HError(6015,"SamplesToPlay: null audio device");
   return OutSamples(a);
}

static void SetVol(AudioOut a, float volume)
{
   if (a==NULL) return;
   if (volume>1.0) volume = 1.0;
   if (trace&T_OUT) printf("HAudio: setting vol to %f\n",volume);

#ifdef UNIX
   {
     int tmp;

     tmp = volume * a->vmax; /*(a->vmax - a->vmin);*/
     if (trace&T_OUT) printf("HAudio: setting vol to %d\n",tmp);
   }
#endif
}

void SetOutVolume(AudioOut a, int volume)
{
   if (a==NULL) HError(6015,"SetVolume: null audio device");
   if (volume<1) volume = 0;
   if (volume>100) volume = 100;
   a->vol = (float) volume / 100.0;
   /* Most machines already log scaled
      a->vol = (volume < 10) ? (float)volume/1000.0 :
      exp (log(10.0)*((float)volume/50.0 - 2.0));
   */
   SetVol(a,a->vol);
}


void InitAudio(void)
{
   int i;
   Boolean b;

   Register(haudio_version);
   numParm = GetConfig("HAUDIO", TRUE, cParm, MAXGLOBS);
   if (numParm>0){
      if (GetConfInt(cParm,numParm, "TRACE",&i)) trace = i;
      if (GetConfBool(cParm,numParm, "ECHOCANCEL",&b)) echoCancelling = b;
      if (GetConfInt(cParm,numParm, "ECHOFILTERSIZE",&i)) echoFilterSize = i;
      if (GetConfInt(cParm,numParm, "ECHOFILTERAWIN",&i)) echoFilterAWin = i;
      if (GetConfInt(cParm,numParm, "ECHOMAXDELAY",&i)) echoMaxDelay = i;
   }
   if (trace&T_DMP) {
      dumpf = fopen("dumpfile.wav","wb");
      if (dumpf==NULL)
         HError(999,"Cannot open dumpfile to write");
   }
}

static void PlayAudio(AudioOut a, short *buf, int nSamples, Boolean isLast)
{
#ifdef UNIX
   /* alloc an alsabuf */
   alsaBuf *aBuf;
   int i;
   static int id=0;

   if (trace & T_OUT) {
      printf(" Putting %d sample block on ALSA audio output queue",nSamples);
      if (isLast) printf(" - the last");
      printf("\n");
   }
   assert(nSamples>0);
   assert(nSamples==WAVEPACKETSIZE || isLast);
   aBuf=(alsaBuf*)New(a->mem, sizeof(alsaBuf));
   aBuf->n=WAVEPACKETSIZE;
   aBuf->isLast=isLast;      /* used on output to signal end of utterance */
   for(i=0;i<nSamples;i++)
     aBuf->waveData[i]=buf[i];
   /* last output packet might not be full, so pad it with zeros */
   if (nSamples<WAVEPACKETSIZE){
     for (i=nSamples; i<WAVEPACKETSIZE; i++)
       aBuf->waveData[i]=0;
   }
   aBuf->next=NULL; aBuf->id=id++;
   HAudioLock();
   a->callThreadId=HThreadSelf();
   /* check queue status - if head empty*/
   if(a->toPlayHead==NULL) {
     /* set as head/tail */
     //snd_pcm_prepare(a->pcm_handle);
     a->toPlayHead=a->toPlayTail=aBuf;
     /* clear any remaining "now playing" packets? */
     /*a->playingHead=a->playingTail=NULL;*/
     a->nBlocks=a->total=aBuf->n;
     a->stored=a->played=0;
     }
   else {
     a->toPlayTail->next=aBuf;
     a->toPlayTail=aBuf;
     a->nBlocks++; a->total+=aBuf->n;
   }
   //alsa_play_from_queue(a);
   HAudioUnlock();
#endif
   a->isActive = TRUE;
}

void PlayAudioOutput(AudioOut a, long nSamples, short *buf,
                     Boolean isLast, AudioIn ain)
{
   if (trace&T_TOP && !a->isActive)
      printf("HAudio: playing new utterance\n");
   if(trace&T_TOP && isLast)
      printf("HAudio: playing last block of utterance\n");
   if (a==NULL) HError(6015,"PlayAudioOutput: null audio device");
   if (a->vol >= 0) SetVol(a,a->vol);
   a->ain = ain;
   PlayAudio(a,buf,nSamples,isLast);
}

AudioOut OpenAudioOutput(HTime *sampPeriod)
{
   AudioOut a;

   if (trace&T_TOP)
      printf("HAudio: opening audio output - sampP=%.0f\n",*sampPeriod);
   a = (AudioOut)malloc(sizeof(AudioOutRec));
   a->vol = -1;
   //InitAudo(a,sampPeriod);
   return a;
}

void FlushAudo(AudioOut a)
{
   a->isActive = FALSE;
#ifdef UNIX
   alsaBuf *aBuf;

   HAudioLock();
   aBuf=a->toPlayHead;
   while(aBuf) {
     a->toPlayHead=a->toPlayHead->next;
     if (aBuf->isLast && a->callThreadId != 0) {
       HEventRec r;
       r.event=HAUDOUT;
       postEventToQueue(a->callThreadId,r);
       if(trace&T_AUD)
	 printf("HAudio: flushing, sent last block!\n");
     }
     Dispose(a->mem,aBuf);
     aBuf=a->toPlayHead;
   }
   a->toPlayHead=a->toPlayTail=NULL;
   a->total=0; a->nBlocks=0;
   a->stored=a->played=0;
   HAudioUnlock();
#endif
}

void FlushAudioOutput(AudioOut a)
{
   if (trace&T_TOP)
      printf("HAudio: killing audio output\n");
   FlushAudo(a);
}

float GetCurrentVol(AudioIn a)
{
   float v,knee;
   if (a==NULL) return 0.0;
   knee = 10000.0;
   if (a->curVol<knee)
      v = (a->curVol*50)/knee;
   else
      v = 50.0 * (1 + (a->curVol-knee)/(65536.0 - knee));
   if (v<0.0) v = 0.0; else if (v>100.0) v = 100.0;
   return v;
}