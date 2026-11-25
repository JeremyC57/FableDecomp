// CCriticalSectionLock_Methods::Lock  @ 00a62980
// Enters the wrapped critical section once (idempotent via a "locked" flag).

#include "engine/CCriticalSectionLock.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/Win32Types.h"           // EnterCriticalSection (kernel32 import, IAT 0x0143FE0C)
// Import (NF415, 2026-11-25): was `(*g_pfnEnter)(CriticalSection)` through an extern const pointer holding the baked IAT slot
//   0x0143FE0C; the slot is KERNEL32!EnterCriticalSection (Fable.exe import directory; reloc_bindings __imp__EnterCriticalSection@4
//   -> 0143fe0c; engine/Win32Types.h declares it). The pointer object g_pfnEnter (unread data, no table line) is gone.

struct CCriticalSectionLock_Methods : CCriticalSectionLock {
    // +0x04 : already-entered flag

    void Lock();
};

void CCriticalSectionLock_Methods::Lock()
{
    if (!Locked)
    {
        EnterCriticalSection(CriticalSection);
        Locked = 1;
    }
}