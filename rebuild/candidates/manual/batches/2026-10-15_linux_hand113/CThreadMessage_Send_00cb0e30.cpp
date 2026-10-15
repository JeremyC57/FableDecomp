// CThreadMessage::Send(unsigned long) -- under the message lock: fails with E_FAIL when no worker thread is running, else posts the message, signals the request event, blocks on the reply event and returns the reply the worker stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0E30, length 84 bytes.
// Callees: EnterCriticalSection (IAT 0x0143FE0C), LeaveCriticalSection (IAT 0x0143FE10, twice),
// SetEvent (IAT 0x0143FDE0), WaitForSingleObject (IAT 0x0143FEAC).
// Layout (no PDB layout; read from this class's neighbours): +0x04 request event, +0x08 reply event,
// +0x0c posted message, +0x10 reply value (written by the reply member 0x00CB0EE0), +0x14 worker thread
// handle (CreateThread result, 0x00CB0DD0), +0x18 the CRITICAL_SECTION.
// Shape: `&m_lock` is formed once into edi and reused by every lock call; the reply is read into a local
// (esi) before the unlock so the return value survives the call. Imports are dllimport.
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                               // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC

struct CThreadMessage {
    void*                 m_vtable;                // +0x00
    void*                 m_hRequestEvent;         // +0x04
    void*                 m_hReplyEvent;           // +0x08
    unsigned long         m_message;               // +0x0c
    long                  m_reply;                 // +0x10
    void*                 m_hThread;               // +0x14
    _RTL_CRITICAL_SECTION m_lock;                  // +0x18
    long Send(unsigned long message);
};

long CThreadMessage::Send(unsigned long message)
{
    EnterCriticalSection(&m_lock);
    if (m_hThread == 0) {
        LeaveCriticalSection(&m_lock);
        return 0x80004005L;                        // E_FAIL
    }
    m_message = message;
    SetEvent(m_hRequestEvent);
    WaitForSingleObject(m_hReplyEvent, 0xFFFFFFFF);
    long reply = m_reply;
    LeaveCriticalSection(&m_lock);
    return reply;
}
