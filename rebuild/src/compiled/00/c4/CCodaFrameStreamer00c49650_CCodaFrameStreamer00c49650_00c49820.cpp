// CCodaFrameStreamer_00c49650::CCodaFrameStreamer_00c49650(unsigned long channels, long rate, unsigned long frames, unsigned long samples, float volume) -- EH-framed constructor of the GameCODA frame streamer: build the frame queue, the done-mailbox and the free-frame stack for the frame count, warn when it is zero (CCodaFrameStreamer.cpp line 105), new every 16-bit frame (Null frame warning, line 111) and post it, then under the sound lock create the streaming source, set its volume, start it with the frame callback and enable it
// Leaf: ??0CCodaFrameStreamer_00c49650@@QAE@KJKKM@Z
// Vendor naming V2 (plan phase 1, header-first): PKCSemaphore comes from the shared libpk/PKCSemaphore.h (was a local copy).
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49820, 581 bytes, thiscall `ret 14h`, /EHsc.
// Identity: libpk .\CCodaFrameStreamer.cpp (lines 105, 111); vtable 0x012B8138 over base 0x012B80AC as the landed dtor 0x00C49650.
// Callees: PKCStaticQueue<CCodaFrame*> 0x00C49E00, the mailbox ctor 0x00C49B30 / ProcessMessage 0x00C49B90, the frame stack
//   0x00C49C90, CCodaFrame 0x00C491E0, the source helpers 0x00C35EC0 / 0x00C35AF0 / 0x00C359E0 / 0x00C35DD0 (extern "C":
//   the lock carries no EH state), OnFrameDone 0x00C497E0.
// Shape: frames newed and posted in the body, then the source set up under the sound lock 0x0143BAFC.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c49820)(const char* text);
extern PFNLOGSINK_00c49820 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                       // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
};
struct CDebugBreak {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CDebugBreak();                         // 0x00C16750
};
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
#include "libpk/PKCSemaphore.h"

class CCodaFrame {                          // the 16-bit PCM frame (.\CFrame.cpp), 0x14 bytes
public:
    unsigned long m_channels;                   // +0x00
    unsigned long m_samples;                    // +0x04
    unsigned long m_validSamples;               // +0x08
    unsigned long m_position;                   // +0x0c
    short*        m_pData;                      // +0x10
    CCodaFrame(unsigned long channels, unsigned long samples);   // 0x00C491E0
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};
template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C49E00
    virtual void Pop(T* item);                  // slot 0 (0x00C49F10)
    virtual long Push(const T* item);           // slot 1 (0x00C49E30)
    virtual unsigned long GetCount();           // slot 2 (0x00C49B10)
    virtual long IsFull();                      // slot 3 (0x00C49B20)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C49AF0
};

struct CMailboxQueue : PKCCriticalSection {     // 0x20 bytes: the queue it drives at +0x18, semaphore at +0x1c
    void*        m_pQueue;                      // +0x18
    PKCSemaphore m_semaphore;                   // +0x1c
    CMailboxQueue(void* queue, long capacity);  // 0x00C49B30
    long ProcessMessage(void* message);         // 0x00C49B90
};

struct SFrame;
struct CFrameStack : PKCCriticalSection {       // 0x28 bytes: count, slot block, used count, top
    long  m_count;                              // +0x18
    void* m_pIndices;                           // +0x1c
    long  m_used;                               // +0x20
    void* m_pTop;                               // +0x24
    CFrameStack(long count);                    // 0x00C49C90
    ~CFrameStack();                             // 0x00C49D00
};

struct CAudioObject;
extern "C" CAudioObject* __stdcall CreateSource_00c35ec0(unsigned long flags);                      // 0x00C35EC0
extern "C" void __stdcall CODA_SetSourceVolume(CAudioObject* source, float volume);       // 0x00C35AF0
struct CCodaFrameStreamer_00c49650;
typedef void (__stdcall* PFNFRAMEDONE_00c49820)(void* handle, void* buffer, CCodaFrameStreamer_00c49650* streamer);
extern "C" int __stdcall AudioObject_Play(CAudioObject* object, unsigned long frames, PFNFRAMEDONE_00c49820 callback, CCodaFrameStreamer_00c49650* context);   // 0x00C359E0
extern "C" long __stdcall CODA_StopSource(CAudioObject* source, long flag);              // 0x00C35DD0

struct CStreamerBase_00c49650 {            // vtable 0x012B80AC
    virtual ~CStreamerBase_00c49650() { }
};

struct CCodaFrameStreamer_00c49650 : CStreamerBase_00c49650 {   // vtable 0x012B8138
    CAudioObject* m_pSource;               // +0x04
    PKCStaticQueue<CCodaFrame*> m_frameQueue;   // +0x08
    CMailboxQueue m_doneQueue;             // +0x1c
    unsigned long m_frames;                // +0x3c
    long          m_rate;                  // +0x40
    unsigned long m_channels;              // +0x44
    CFrameStack   m_freeFrames;            // +0x48
    long          m_pending;               // +0x70
    CCodaFrameStreamer_00c49650(unsigned long channels, long rate, unsigned long frames, unsigned long samples, float volume);
    virtual ~CCodaFrameStreamer_00c49650();   // 0x00C49650
    static void __stdcall OnFrameDone(void* handle, void* buffer, CCodaFrameStreamer_00c49650* streamer);   // 0x00C497E0
};

CCodaFrameStreamer_00c49650::CCodaFrameStreamer_00c49650(unsigned long channels, long rate, unsigned long frames, unsigned long samples, float volume)
    : m_pSource(0),
      m_frameQueue(frames),
      m_doneQueue(&m_frameQueue, frames),
      m_frames(frames),
      m_rate(rate),
      m_channels(channels),
      m_freeFrames(frames),
      m_pending(0)
{
    if (m_frames == 0) {
        {
            CString line;
            line << ".\\CCodaFrameStreamer.cpp" << "(" << 105L << ") : " << "Expecting at least one frame.. sniff sniff" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    for (unsigned long i = 0; i < m_frames; ++i) {
        CCodaFrame* frame = new CCodaFrame(m_channels, samples);
        if (!frame) {
            {
                CString line;
                line << ".\\CCodaFrameStreamer.cpp" << "(" << 111L << ") : " << "Null frame" << "\n";
                if (g_pfnLogSink_0143bb2c)
                    g_pfnLogSink_0143bb2c(line.m_pchData);
            }
            CDebugBreak();
        }
        m_doneQueue.ProcessMessage(&frame);
    }
    PKCProtectionInstance lock(&g_soundLock);
    m_pSource = CreateSource_00c35ec0(0x10003);
    CODA_SetSourceVolume(m_pSource, volume);
    AudioObject_Play(m_pSource, m_frames, OnFrameDone, this);
    CODA_StopSource(m_pSource, 1);
}