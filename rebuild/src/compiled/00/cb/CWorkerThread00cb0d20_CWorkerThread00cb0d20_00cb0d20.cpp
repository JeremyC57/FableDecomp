// CWorkerThread_00cb0d20::~CWorkerThread_00cb0d20() -- take the thread handle at +0x14 with an interlocked exchange, wait for it and close it when there was one, delete the two critical sections at +0x30 and +0x18, then close the event handles at +8 and +4 (each when held)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0D20, length 83 bytes.
// Callees: KERNEL32 InterlockedExchange (IAT 0x0143FF00, once), WaitForSingleObject (IAT 0x0143FEAC, once), CloseHandle (IAT 0x0143FE30, three times through ebx),
// DeleteCriticalSection (IAT 0x0143FDE4, twice through edi). Shape: the imports used more than once are loaded into registers (`__declspec(dllimport)`), the
// single-use ones are `call dword ptr [__imp]`; the last handle is loaded into esi. The chunk label is propagated; the class is named by address.
extern "C" __declspec(dllimport) long __stdcall InterlockedExchange(long volatile* target, long value);   // IAT 0x0143FF00
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                      // IAT 0x0143FE30
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);   // IAT 0x0143FDE4

struct CWorkerThread_00cb0d20 {
    long                  m_0;
    void*                 m_hEventA;               // +0x04
    void*                 m_hEventB;               // +0x08
    unsigned char         _pad0c[8];
    long volatile         m_hThread;               // +0x14
    _RTL_CRITICAL_SECTION m_sectionA;              // +0x18
    _RTL_CRITICAL_SECTION m_sectionB;              // +0x30
    ~CWorkerThread_00cb0d20();
};

CWorkerThread_00cb0d20::~CWorkerThread_00cb0d20()
{
    void* thread = (void*)InterlockedExchange(&m_hThread, 0);
    if (thread) {
        WaitForSingleObject(thread, 0xffffffff);
        CloseHandle(thread);
    }
    DeleteCriticalSection(&m_sectionB);
    DeleteCriticalSection(&m_sectionA);
    if (m_hEventB)
        CloseHandle(m_hEventB);
    if (m_hEventA)
        CloseHandle(m_hEventA);
}