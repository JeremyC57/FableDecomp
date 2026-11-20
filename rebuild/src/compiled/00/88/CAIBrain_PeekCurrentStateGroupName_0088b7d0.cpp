// CAIBrain::PeekCurrentStateGroupName() const -- the active state group's name, or null when none is active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B7D0: `mov eax,[ecx+38h] ; test eax,eax ; je ; add eax,0Ch ; ret` (PActiveStateGroup +0x38,
//   CAIStateGroupBase::Name +0x0c).
// Leaf: ?PeekCurrentStateGroupName@CAIBrain@@QBEPBVCCharString@@XZ
// NF123 (2026-11-20, q6435 + q6439): was the stand-in `char* __fastcall DrawGetWorldMap(S*)` (catalog CEditWorld::DrawGetWorldMap). Ego_r.pdb +
//   FableWin.pdb name sets ?PeekCurrentStateGroupName@CAIBrain@@QBEPBVCCharString@@XZ (Ego_r donor order 0x0082F5A0 / 0x0082F5B0 matches the
//   retail pair 0x0088B7D0 / 0x0088B7E0 GetCurrentStateGroupDebugText; Ego_r 0x0082F5A0 + FableWin 0x02805A90); the retail caller 0x00833C00
//   (not a landed row) calls it on CThingAICreature::PBrain.
//   Ported onto engine/CAIBrain.h (opt-in FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT) and engine/CAIStateGroupBase.h.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

const CCharString* CAIBrain::PeekCurrentStateGroupName() const
{
    return PActiveStateGroup ? &PActiveStateGroup->Name : 0;
}
