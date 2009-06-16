#ifndef AQCODE_H
#define AQCODE_H

#include <AudioToolbox/AudioToolbox.h>

static const int kNumberBuffers = 3;                            // 1

struct AQRecorderState {
    AudioStreamBasicDescription  mDataFormat;                   // 2
    AudioQueueRef                mQueue;                        // 3
    AudioQueueBufferRef          mBuffers[kNumberBuffers];      // 4
    AudioFileID                  mAudioFile;                    // 5
    UInt32                       bufferByteSize;                // 6
    SInt64                       mCurrentPacket;                // 7
    bool                         mIsRunning;                    // 8
};

class Recorder {
public:
	Recorder();
	void start();
	void stop();
	bool isRunning();
private:
	AQRecorderState aqData;
	AudioFileTypeID fileType;
	CFURLRef audioFileURL;
};

void HandleInputBuffer (
    void                                 *aqData,
    AudioQueueRef                        inAQ,
    AudioQueueBufferRef                  inBuffer,
    const AudioTimeStamp                 *inStartTime,
    UInt32                               inNumPackets,
    const AudioStreamPacketDescription   *inPacketDesc
);

void DeriveBufferSize (
    AudioQueueRef                audioQueue,                  // 1
    AudioStreamBasicDescription  &ASBDescription,             // 2
    Float64                      seconds,                     // 3
    UInt32                       *outBufferSize               // 4
);

OSStatus SetMagicCookieForFile (
    AudioQueueRef inQueue,                                      // 1
    AudioFileID   inFile                                        // 2
);

#endif