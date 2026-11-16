// CAnimSet::IsFinished() const -- true when the set holds no animation or its Finished bit (+0x51 & 2) is set
// Leaf: ?IsFinished@CAnimSet@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00668A80 (a COMDAT of the creature TU, called by 0x00665B5A).
// NF39 (2026-11-16): moved off its free __fastcall stand-in (CAnimateThing_IsAnimationFinished over a local CAnimateThing) onto the shared
//   engine/CAnimSet.h (opt-in FABLE_RETAIL_CANIMSET_TYPE, which names 0x00668A80): Ego_r.pdb + FableWin.pdb ?IsFinished@CAnimSet@@QBE_NXZ;
//   Anims (vector<short>) +4 / +8, Finished bit +0x51; its landed caller CThingCreatureBase::UpdateAnimation 0x00665860 already binds it.
#define FABLE_RETAIL_CANIMSET_TYPE
#include "engine/CAnimSet.h"

bool CAnimSet::IsFinished() const
{
    return Anims.empty() || Finished;
}
