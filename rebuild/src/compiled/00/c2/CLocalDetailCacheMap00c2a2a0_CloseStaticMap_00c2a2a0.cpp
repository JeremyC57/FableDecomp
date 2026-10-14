// CLocalDetailCacheMap_00c2a2a0::CloseStaticMap() -- wait on the event at +4 without a timeout (0x00C36150 with -1), sleep 20 ms (Sleep @ IAT 0x0143FE1C), close the handle at +0xc (CloseHandle @ IAT 0x0143FE30) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A2A0, length 48 bytes.
// The chunk label is propagated; named by address. Shape: `this` in esi, the three calls in order, `ret`.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);          // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                  // IAT 0x0143FE30

struct CWaitableEvent_00c36150 {                   // a handle: Wait(timeout) is WaitForSingleObject != WAIT_TIMEOUT
    void* m_handle;
    unsigned char _pad04[4];
    bool Wait(unsigned long timeout);              // 0x00C36150
};

struct CLocalDetailCacheMap_00c2a2a0 {
    long                    m_0;
    CWaitableEvent_00c36150 m_finished;            // +0x04
    void*                   m_hThread;             // +0x0c
    void CloseStaticMap();
};

void CLocalDetailCacheMap_00c2a2a0::CloseStaticMap()
{
    m_finished.Wait(0xffffffff);
    Sleep(20);
    CloseHandle(m_hThread);
    m_hThread = 0;
}