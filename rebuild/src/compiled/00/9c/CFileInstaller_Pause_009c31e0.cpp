// CFileInstaller::Pause() -- under the installer's critical section (+4): bump the pause count (+0x7c); when the installer thread is waiting (+0x7b), this is the first pause and no dirty-disc error is pending, drop the lock, signal the event at +0x84 and block (alertable, INFINITE) on the handle at +0x8c until the installer thread acknowledges
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C31E0, length 103 bytes (+ int3 padding).
// Leaf: ?Pause@CFileInstaller@@QAEXXZ
// Callees: CCriticalSectionLock::CCriticalSectionLock @ 0x00A62920 (the section, false), CCriticalSectionLock::Unlock @ 0x00A62960,
// ~CCriticalSectionLock @ 0x00A629A0 (an 8-byte stack local), KERNEL32 SetEvent (IAT 0x0143FDE0), KERNEL32 WaitForSingleObjectEx (IAT 0x0143FE08).
// Global: the static bool CFileInstaller::DirtyDiscError @ 0x013CA814 (tested `mov al,[g] ; test al,al ; jne`): the only
// writer is the static CFileInstaller::SetDirtyDiscError(bool) 0x009C3530 (Ego_r.pdb ?SetDirtyDiscError@CFileInstaller@@
// KIX_N@Z; `mov [0x013CA814],bl` under the installer lock), and the read/write/decompress paths test it (FINDINGS chunk
// 411). The member name is descriptive (the PDB name sets carry no static data names).
// Shape: twin of the landed CFileInstaller::Unpause 0x009C3250: the count is volatile (load/inc/store, then re-read for the
// unsigned `cmp 1 ; ja`), the waiting flag byte read after the store; the lock released explicitly before the blocking wait
// (as CBankFileAsyncData::WaitUntilFinished 0x009D5B60) and destroyed again on the way out.
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                                            // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long ms, int alertable);   // IAT 0x0143FE08

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };

struct CCriticalSectionLock {              // 8 bytes on the stack
    _RTL_CRITICAL_SECTION* CriticalSection;       // +0x00
    bool                    Locked;                // +0x04
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920 ??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z
    ~CCriticalSectionLock();                                               // 0x00A629A0
    void Unlock();                                                                // 0x00A62960
};

class CFileInstaller { public:
    unsigned char          _pad[4];
    _RTL_CRITICAL_SECTION CriticalSection;              // +0x04
    unsigned char          _pad1c[0x5f];
    volatile bool          Running;              // +0x7b
    volatile unsigned long Paused;           // +0x7c
    unsigned char          _pad80[4];
    void*                  Event;         // +0x84
    unsigned char          _pad88[4];
    void*                  PauseStartEvent;        // +0x8c
    static bool            DirtyDiscError;         // 0x013CA814 (descriptive name)
    void Pause();
};

void CFileInstaller::Pause()
{
    CCriticalSectionLock lock(CriticalSection, false);
    ++Paused;
    if (Running && !(Paused > 1) && !DirtyDiscError) {
        lock.Unlock();
        SetEvent(Event);
        WaitForSingleObjectEx(PauseStartEvent, 0xFFFFFFFF, 1);
    }
}