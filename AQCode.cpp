#include "AQCode.h"

Recorder::Recorder()
{
	aqData.mDataFormat.mFormatID = kAudioFormatLinearPCM;        // 2
	aqData.mDataFormat.mSampleRate = 16000.0;                    // 3
	aqData.mDataFormat.mChannelsPerFrame = 1;                    // 4
	aqData.mDataFormat.mBitsPerChannel = 16;                     // 5
	aqData.mDataFormat.mBytesPerPacket =                         // 6
    aqData.mDataFormat.mBytesPerFrame =
	aqData.mDataFormat.mChannelsPerFrame * sizeof (SInt16);
	aqData.mDataFormat.mFramesPerPacket = 1;                     // 7

	fileType = kAudioFileWAVEType;                               // 8
	aqData.mDataFormat.mFormatFlags =                            // 9
    kLinearPCMFormatFlagIsSignedInteger
    | kLinearPCMFormatFlagIsPacked;
}

void Recorder::start()
{
	AudioQueueNewInput (                                              // 1
						&aqData.mDataFormat,                          // 2
						HandleInputBuffer,                            // 3
						&aqData,                                      // 4
						NULL,                                         // 5
						kCFRunLoopCommonModes,                        // 6
						0,                                            // 7
						&aqData.mQueue                                // 8
	);

	UInt32 dataFormatSize = sizeof (aqData.mDataFormat);       // 1
	 
	AudioQueueGetProperty (                                    // 2
	  aqData.mQueue,                                           // 3
	  kAudioConverterCurrentOutputStreamDescription,           // 4
	  &aqData.mDataFormat,                                     // 5
	  &dataFormatSize                                          // 6
	);
	
	const char *filePath = "recording.wav";
	
	audioFileURL =
		CFURLCreateFromFileSystemRepresentation (            // 1
			NULL,                                            // 2
			(const UInt8 *) filePath,                        // 3
			strlen (filePath),                               // 4
			false                                            // 5
		);
	AudioFileCreateWithURL (                                 // 6
		audioFileURL,                                        // 7
		fileType,                                            // 8
		&aqData.mDataFormat,                                 // 9
		kAudioFileFlags_EraseFile,                           // 10
		&aqData.mAudioFile                                   // 11
	);
	
	DeriveBufferSize (                               // 1
		aqData.mQueue,                               // 2
		aqData.mDataFormat,                          // 3
		0.5,                                         // 4
		&aqData.bufferByteSize                       // 5
	);

	for (int i = 0; i < kNumberBuffers; ++i) {           // 1
		AudioQueueAllocateBuffer (                       // 2
			aqData.mQueue,                               // 3
			aqData.bufferByteSize,                       // 4
			&aqData.mBuffers[i]                          // 5
		);
	 
		AudioQueueEnqueueBuffer (                        // 6
			aqData.mQueue,                               // 7
			aqData.mBuffers[i],                          // 8
			0,                                           // 9
			NULL                                         // 10
		);
	}

	aqData.mCurrentPacket = 0;                           // 1

	aqData.mIsRunning = true;

	AudioQueueStart (                                    // 3
		aqData.mQueue,                                   // 4
		NULL                                             // 5
	);
}

void Recorder::stop()
{
	AudioQueueStop (                                     // 6
		aqData.mQueue,                                   // 7
		true                                             // 8
	);
	aqData.mIsRunning = false;                          // 9
	AudioQueueDispose (                                 // 1
		aqData.mQueue,                                  // 2
		true                                            // 3
	);
 
	AudioFileClose (aqData.mAudioFile);                 // 4
}

bool Recorder::isRunning()
{
	return aqData.mIsRunning;
}

void HandleInputBuffer (
    void                                 *aqData,
    AudioQueueRef                        inAQ,
    AudioQueueBufferRef                  inBuffer,
    const AudioTimeStamp                 *inStartTime,
    UInt32                               inNumPackets,
    const AudioStreamPacketDescription   *inPacketDesc
) {
    AQRecorderState *pAqData = (AQRecorderState *) aqData;               // 1
 
    if (inNumPackets == 0 &&                                             // 2
          pAqData->mDataFormat.mBytesPerPacket != 0)
       inNumPackets =
           inBuffer->mAudioDataByteSize / pAqData->mDataFormat.mBytesPerPacket;
 
    if (AudioFileWritePackets (                                          // 3
            pAqData->mAudioFile,
            false,
            inBuffer->mAudioDataByteSize,
            inPacketDesc,
            pAqData->mCurrentPacket,
            &inNumPackets,
            inBuffer->mAudioData
        ) == noErr) {
            pAqData->mCurrentPacket += inNumPackets;}                    // 4
 
   if (pAqData->mIsRunning == 0)                                         // 5
      return;
 
    AudioQueueEnqueueBuffer (                                            // 6
        pAqData->mQueue,
        inBuffer,
        0,
        NULL
    );
}

void DeriveBufferSize (
    AudioQueueRef                audioQueue,                  // 1
    AudioStreamBasicDescription  &ASBDescription,             // 2
    Float64                      seconds,                     // 3
    UInt32                       *outBufferSize               // 4
) {
    static const int maxBufferSize = 0x50000;                 // 5
 
    int maxPacketSize = ASBDescription.mBytesPerPacket;       // 6
    if (maxPacketSize == 0) {                                 // 7
        UInt32 maxVBRPacketSize = sizeof(maxPacketSize);
        AudioQueueGetProperty (
                audioQueue,
                kAudioConverterPropertyMaximumOutputPacketSize,
                &maxPacketSize,
                &maxVBRPacketSize
        );
    }
 
    Float64 numBytesForTime =
        ASBDescription.mSampleRate * maxPacketSize * seconds; // 8
    *outBufferSize =
    UInt32 (numBytesForTime < maxBufferSize ?
        numBytesForTime : maxBufferSize);                     // 9
}

OSStatus SetMagicCookieForFile (
    AudioQueueRef inQueue,                                      // 1
    AudioFileID   inFile                                        // 2
) {
    OSStatus result = noErr;                                    // 3
    UInt32 cookieSize;                                          // 4
 
    if (
            AudioQueueGetPropertySize (                         // 5
                inQueue,
                kAudioQueueProperty_MagicCookie,
                &cookieSize
            ) == noErr
    ) {
        char* magicCookie =
            (char *) malloc (cookieSize);                       // 6
        if (
                AudioQueueGetProperty (                         // 7
                    inQueue,
                    kAudioQueueProperty_MagicCookie,
                    magicCookie,
                    &cookieSize
                ) == noErr
        )
            result =    AudioFileSetProperty (                  // 8
                            inFile,
                            kAudioFilePropertyMagicCookieData,
                            cookieSize,
                            magicCookie
                        );
        free (magicCookie);                                     // 9
    }
    return result;                                              // 10
}
