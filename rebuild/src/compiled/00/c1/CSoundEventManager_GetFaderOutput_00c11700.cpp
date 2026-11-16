// CSoundEventManager::GetFaderOutput() const -- the manager's fader output (its +0x7D8 CFader's ramped value times the fade level)
// Leaf: ?GetFaderOutput@CSoundEventManager@@QBEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11700, 11 bytes: `add ecx,7D8h ; jmp 0x00C25F00` (thiscall tail call, the float result
//   left on the FPU stack by the callee).
// Identity: DESCRIPTIVE name (libpk has no PDB or RTTI; recorded in NOTES). The owner is CSoundEventManager: the only caller, the
//   landed 0x00C271E0, is CPhysicalPlayer::UpdateManagerTime(CSoundEventManager*) as the landed CPhysicalPlayer::Play 0x00C271F0
//   declares and binds it, and the manager's constructor 0x00C11D00 builds a CFader (CFader::CFader 0x00C26010) at +0x7D8 that its
//   destructor 0x00C13470 destroys. The callee is the landed CFader::GetOutput 0x00C25F00 (`?GetOutput@CFader@@QBEMXZ`, already
//   bound by 0x00C25F40); it has no side effects, so the forwarder is a getter. acctC's NAME_FIXES proposal "GetTime() (double)"
//   is not used: the callee returns the fader's output as a float, not a time.
// Header: libpk/CSoundEventManager.h (FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER), libpk/CFader.h.
#define FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER
#include "libpk/CSoundEventManager.h"

float CSoundEventManager::GetFaderOutput() const
{
    return m_fader.GetOutput();
}