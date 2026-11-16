// CPhysicalPlayer::UpdateManagerTime(CSoundEventManager* manager) -- reads the manager's fader output and drops it
// Leaf: ?UpdateManagerTime@CPhysicalPlayer@@QAEXPAUCSoundEventManager@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C271E0, 14 bytes: `mov ecx,[esp+4] ; call 0x00C11700 ; fstp st(0) ; ret 4` (thiscall, `this`
//   unused). Callers: the landed CPhysicalPlayer::Play 0x00C271F0 (which declares and binds this spelling) and the 5-byte tail-jump
//   0x00C27540.
// Identity: DESCRIPTIVE name, as the landed Play row and acctC's wfA1 NAME_FIXES give it (libpk has no PDB or RTTI; recorded in
//   NOTES). Respelled in landing gm from the propagated `_global::Cons_val(Alloc*, Pair*, Pair*)` / `cons_ctor(Pair*)` stand-in.
//   The body only calls CSoundEventManager::GetFaderOutput 0x00C11700 (a side-effect-free getter in another translation unit)
//   and pops its result: the original probably used the value in code compiled out of the release build.
// Header: libpk/CSoundEventManager.h (FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER), libpk/CFader.h.
#define FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
#include "libpk/CSoundEventManager.h"

struct CPhysicalPlayer {                        // the CODA physical player (vtable 0x012B4034; layout in the landed Play row)
    void UpdateManagerTime(CSoundEventManager* manager);   // 0x00C271E0
};

void CPhysicalPlayer::UpdateManagerTime(CSoundEventManager* manager)
{
    manager->GetFaderOutput();
}
