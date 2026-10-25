// PKCThread::Join() -- wait without a timeout on the waitable handle at +4 (0x00C36150 with INFINITE), sleep 20 ms (Sleep @ IAT 0x0143FE1C), close the thread handle at +0xc (CloseHandle @ IAT 0x0143FE30) and clear it
// Leaf: ?Join@PKCThread@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A2A0, length 40 bytes (+ int3 padding), thiscall `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 "0xC2A2A0 CLocalDetailCacheMap::CloseStaticMap = PKCThread::Join"):
//   its two callers are the libpk worker-queue destructors 0x00C14560 / 0x00C14630 (calls 0x00C145D1 / 0x00C146A1),
//   which join their polymorphic worker thread (+0x2c) and then delete it through its virtual deleting destructor; the
//   +4 helper 0x00C36150 is WaitForSingleObject(handle, timeout) (IAT 0x0143FEAC) returning 1 on WAIT_TIMEOUT. The
//   class name follows agent433's worker-queue model (PKCThread; libpk's ".\PKCThreadManager.cpp" is the thread
//   source file); descriptive member names, no PDB covers libpk.
// Label correction: the landed row was CLocalDetailCacheMap_00c2a2a0::CloseStaticMap (a propagated game label) over a
//   non-polymorphic layout with a placeholder word at +0.
// Shape: `this` in esi, the three calls in order, `ret`.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);          // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                  // IAT 0x0143FE30

struct CWaitableHandle {                           // 8 bytes: a Win32 handle
    void* m_handle;
    unsigned char _pad04[4];
    long Wait(unsigned long timeout);              // 0x00C36150: 1 when the wait timed out
};

class PKCThread {                                  // polymorphic worker thread
public:
    virtual ~PKCThread();                          // slot 0
    CWaitableHandle m_finished;                    // +0x04
    void*           m_hThread;                     // +0x0c
    void Join();
};

void PKCThread::Join()
{
    m_finished.Wait(0xffffffff);
    Sleep(20);
    CloseHandle(m_hThread);
    m_hThread = 0;
}
