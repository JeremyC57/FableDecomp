// CAutoLock::CAutoLock(CCritSec* plock) -- DirectShow base classes (strmbase wxutil.h): remember the critical section and enter it
// Leaf: ??0CAutoLock@@QAE@PAVCCritSec@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA48A0, 22 bytes (+10 int3), __fastcall, `ret 4`, returns `this`.
// Identity: the body is strmbase's inline `CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }` with
//   CCritSec::Lock = EnterCriticalSection(&m_CritSec) (IAT 0x0143FE0C), emitted out of line in the strmbase TU; its one
//   caller is 0x00CA7299 in the DirectShow region (0x00CA4xxx-0x00CB0xxx; the landed CBaseFilter::SetSyncSource 0x00CA7680
//   and CBaseRenderer sources model the same inline CAutoLock). No PDB covers strmbase. Label correction: the catalog's
//   "_List_unchecked_const_iterator<...CPendingQuery...>" is propagated (an ABI-identical body).
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C, `call dword ptr [__imp]`).
// Layout: CAutoLock { CCritSec* m_pLock; } (4 bytes); CCritSec { CRITICAL_SECTION m_CritSec; } (release build).
// Shape: the argument is loaded once (eax) and used for both the member store and the pushed &m_CritSec.
struct CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10

class CCritSec {                                         // strmbase wxutil.h
    CRITICAL_SECTION m_CritSec;
public:
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};

class CAutoLock {                                        // strmbase wxutil.h
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock);
    ~CAutoLock() { m_pLock->Unlock(); }
};

CAutoLock::CAutoLock(CCritSec* plock)
{
    m_pLock = plock;
    m_pLock->Lock();
}