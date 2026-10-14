// CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION*, bool tryOnly) -- scoped lock: try-enter (Locked = success) or enter unconditionally (Locked = true)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62920, length 54 bytes.
// Callees: KERNEL32 TryEnterCriticalSection (IAT 0x0143FD30), EnterCriticalSection (IAT 0x0143FE0C).
// Shape: the section pointer is stored and pushed before the branch (the push is common to both calls, so the
// compiler hoists it); `test eax,eax ; setne al ; mov [esi+4],al` is the bool conversion of the try result,
// the else arm stores an immediate 1. Both imports are called through their IAT slots (`__declspec(dllimport)`).
// Layout from engine/CCriticalSectionLock.h (PDB): CriticalSection +0, Locked +4.
#include "engine/CCriticalSectionLock.h"

extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* cs);      // IAT 0x0143FE0C

struct CCriticalSectionLock_Methods : CCriticalSectionLock {
    CCriticalSectionLock_Methods(_RTL_CRITICAL_SECTION* cs, bool tryOnly);
};

CCriticalSectionLock_Methods::CCriticalSectionLock_Methods(_RTL_CRITICAL_SECTION* cs, bool tryOnly)
{
    CriticalSection = cs;
    if (tryOnly) {
        Locked = TryEnterCriticalSection(cs) != 0;
    } else {
        EnterCriticalSection(cs);
        Locked = true;
    }
}
