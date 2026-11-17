#ifndef FABLE_LIBPK_CCODAFRAMESTREAMER_H
#define FABLE_LIBPK_CCODAFRAMESTREAMER_H
#ifdef FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#include "libpk/PKCCriticalSection.h"
#include "libpk/PKCSemaphore.h"
// Identity: native source-file traces CCodaFrame.cpp and CCodaFrameStreamer.cpp.
// No original libpk PDB names exist in either supplied PDB or the CODA SDK.
// Member and operation names below are descriptive unless separately documented.
// Layout: C491E0 initializes channels+0, capacity+4, samples+8, buffer+c, data+10.
// The C491D0 getter and SubmitFrame's PCM byte count corroborate the data type.
class CCodaFrame {
public:
 unsigned long m_channels,m_capacity,m_samples;
 void* m_hBuffer;
 short* m_pData;
 short* GetData();
};
// Layout: C49E00 allocates capacity*4 bytes; C49E30/C49F10 access the pointer queue.
// Four slots at12b80c8: Pop, Push, GetCount, IsFull. Descriptive template operations.
template<class T> class PKCStaticQueue {
public:
 PKCStaticQueue(long capacity);
 virtual T Pop();
 virtual void Push(const T*);
 virtual long GetCount();
 virtual long IsFull();
 unsigned long m_capacity;
 T* m_pSlots;
 unsigned long m_used,m_writeIndex;
};
// Layout: C49B30 constructs the section at0, stores queue+18 and semaphore+1c.
// C49B90 locks, tests the queue's slot3, signals and calls slot1. The existing
// CMailboxQueue binding denotes this distinct concrete mailbox, not C4BC20.
class CMailboxQueue {
public:
 CMailboxQueue(void* queue,long capacity);
 PKCCriticalSection m_criticalSection;
 PKCStaticQueue<CCodaFrame*>* m_pQueue;
 PKCSemaphore m_semaphore;
 long ProcessMessage(void* message);
};
// Layout: C49D50 guards this, reads capacity+18, slots+1c, count+20, tail+24.
class CLockedRingBuffer {
public:
 PKCCriticalSection m_criticalSection;
 long m_capacity;
 CCodaFrame** m_pSlots;
 long m_count,m_tail;
 void Push(CCodaFrame* frame);
#ifdef FABLE_RETAIL_CCODAFRAMESTREAMER_ONFRAMEDONE
 CCodaFrame* Pop(); // 0x00C4A0E0 (guarded)
#endif
};
// Layout: C49820 initializes all members, C49650 destroys the same members.
// Seven slots at12b8138; the destructor is last, not first.
class CCodaFrameStreamer {
public:
 CCodaFrameStreamer(unsigned long channels,long rate,unsigned long frames,unsigned long frameSamples,float volume);
#ifdef FABLE_RETAIL_CCODAFRAMESTREAMER_ONFRAMEDONE
 // 0x00C497E0: the stream handle's __stdcall frame-done callback (registered by the ctor 0x00C49820, userdata = the streamer).
 static void __stdcall OnFrameDone(void* handle,void* buffer,CCodaFrameStreamer* streamer);
#endif
 virtual void SubmitFrame(CCodaFrame* frame);
 virtual long GetFreeFrameCount();
 virtual long GetQueuedFrameCount();
 virtual CCodaFrame* GetFrame();
 virtual long GetSampleRate();
 virtual void SetSourceVolume(float volume);
 virtual ~CCodaFrameStreamer();
 void* m_pSource;
 PKCStaticQueue<CCodaFrame*> m_freeQueue;
 CMailboxQueue m_freeFrames;
 unsigned long m_frames;
 long m_rate;
 unsigned long m_channels;
 CLockedRingBuffer m_inFlight;
 long m_framesOut;
};
#endif
#endif
