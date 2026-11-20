// CEnvironment::UpdateInterpolation() -- per-frame interpolation step: forward to the spell-effect interface's FrameUpdate
// Leaf: ?UpdateInterpolation@CEnvironment@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B32A0: `mov ecx,[ecx+1960h] ; jmp 0x00885010` (void member -> void member tail call).
// Name (NF118, PRIORITY q4757): Ego_r.pdb 0x006EEF40 / FableWin.pdb 0x0244E1B0 ?UpdateInterpolation@CEnvironment@@QAEXXZ; the landed
//   caller CWorld::UpdateFrame 0x004A5A40 binds that spelling here (engine/CEnvironment.h FABLE_RETAIL_CENVIRONMENT). Callee: Ego_r /
//   FableWin ?FrameUpdate@CEnvironmentSpellEffectInterface@@QAEXXZ 0x00885010 (landed), reached through the inline
//   GetSpellEffectInterface() (SpellEffectInterface_ptr +0x1960, scoped_ptr). Was the stand-in T::Run on a padded local struct.
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_FRAMEUPDATE
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterface.h"
void CEnvironment::UpdateInterpolation()
{
    GetSpellEffectInterface().FrameUpdate();
}
