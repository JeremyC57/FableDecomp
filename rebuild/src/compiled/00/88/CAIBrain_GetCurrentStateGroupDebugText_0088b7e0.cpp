// CAIBrain::GetCurrentStateGroupDebugText() const -- the active state group's (PActiveStateGroup, +0x38) debug text (its virtual
//   slot 6 GetDebugText), or 0 with no active state group
// Leaf: ?GetCurrentStateGroupDebugText@CAIBrain@@QBEPBVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B7E0, length 17 bytes (32 with padding). Caller 0x00833E0C (CThingAICreature's
//   brain-debug draw, 0x00833C00).
// Shape: `mov eax,[ecx+38h] ; test eax,eax ; je Z ; mov ecx,eax ; mov eax,[ecx] ; jmp [eax+18h] ; Z: xor eax,eax ; ret`.
// Identity (Naming NF194, 2026-11-20, q6436 / q6440): Ego_r.pdb 0x0082F5B0 + FableWin.pdb 0x02805AC0
//   ?GetCurrentStateGroupDebugText@CAIBrain@@QBEPBVCCharString@@XZ, the member right after PeekCurrentStateGroupName (retail 0x0088B7D0,
//   landed, the same +0x38 test); engine/CAIBrain.h declares it at 0x0088B7E0 (FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT) and
//   CAIStateGroupBase slot 6 is GetDebugText. Was the propagated label CScriptThing::GetCurrentScriptPriority on a local host model.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

const CCharString* CAIBrain::GetCurrentStateGroupDebugText() const
{
    return PActiveStateGroup ? PActiveStateGroup->GetDebugText() : 0;
}
