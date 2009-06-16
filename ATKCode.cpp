#include <iostream>
#include <vector>

#include <APacket.h>
#include <ABuffer.h>
#include <ACode.h>
#include <ADict.h>
#include <AGram.h>
#include <ASource.h>
#include <ARec.h>
#include <ARMan.h>
#include <HAudio.h>
#include <HThreads.h>
#include <time.h>

#include "ATKCode.h"

std::vector<std::string> recognize() {

	ABuffer auChan("auChan");		// waveform data 
	ABuffer feChan("feChan");		// features 
	ABuffer ansChan("ansChan");		// recognition output 

	ASource ain("AIn",&auChan);			// auChan connects source to coder 
	ACode acode("ACode",&auChan,&feChan); //feChan connects coder to reco 
	
	AHmms hset("HmmSet");	// create HMM set called HmmSet 
	ADict dict("ADict");	// create a dictionary called ADict 
	AGram gram("AGram");	// create a grammar called AGram 
	ARMan rman;				// create a resource manager 
	rman.StoreHMMs(&hset);	// store the resources in rman 
	rman.StoreDict(&dict); 
	rman.StoreGram(&gram); 
	
	ResourceGroup *group = rman.NewGroup("directions"); 
	group->AddHMMs(&hset); 
	group->AddDict(&dict); 
	group->AddGram(&gram); 
	ARec arec("ARec",&feChan,&ansChan,&rman); // create recogniser 
	
	ain.Start(); acode.Start(); arec.Start();
	
	arec.SendMessage("start()");
	ain.SendMessage("start()");

	// Waiting for processing to finish
	do
	{
		usleep(10000);
	}while(!auChan.IsEmpty() || !feChan.IsEmpty());
	
	// Get the results
	std::vector<std::string> result;
	while(!ansChan.IsEmpty())
	{
		APacket p = ansChan.GetPacket();
		if(p.GetKind() == PhrasePacket)
		{
			APhraseData *pd = (APhraseData *)p.GetData();
			result.push_back(pd->word);
		}
	}
	
	ain.SendMessage("stop()");
	acode.SendMessage("terminate()");
	arec.SendMessage("terminate()");
	ain.SendMessage("terminate()");
	
	// Wait for threads to finish
	acode.Join();
	arec.Join();
	ain.Join();
	
	return result;
}

int inithtk(int argc, char *argv[],const char * app_version, bool noGraphics)
{
	InitThreads(HT_MSGMON);   // enable msg driven monitoring
	if(InitShell(argc,argv,app_version)<SUCCESS) return FAIL;
	// CommonInit() expanded with incompatible things commented out: 
		InitMem();   InitLabel();
		InitMath();  InitSigP(); InitUtil();
		InitWave();  /*InitAudio();*/ InitModel();
		if(InitParm()<SUCCESS) return FAIL;
		InitGraf((Boolean)noGraphics);
		InitDict();  InitNet();   InitLM();
		InitRec();  InitLat();  /*InitAdapt();*/
		
		EnableBTrees();   /* allows unseen triphones to be synthesised */
		/*
		Register(abuffer_version);
		Register(acode_version);
		Register(acomponent_version);
		Register(adict_version);
		Register(agram_version);
		Register(angram_version);
		Register(ahmms_version);
		Register(ahtk_version);
		Register(amonitor_version);
		Register(apacket_version);
		Register(arec_version);
		Register(aresource_version);
		Register(arman_version);
		Register(asource_version);
		*/
		InfoPrinted();
		return SUCCESS;
}