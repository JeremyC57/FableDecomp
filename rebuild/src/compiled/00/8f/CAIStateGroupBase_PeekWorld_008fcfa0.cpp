// CAIStateGroupBase::PeekWorld() const -- the brain's thing manager's world reference (PBrain +0x04 -> ThingManager +0x1c -> World +0x24)
// Leaf: ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCFA0, oracle length 10 bytes.
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [eax + 0x1c] ; mov eax, dword ptr [ecx + 0x24] ; ret
// Naming (agentNF31, 2026-11-12): was a tiny-body stand-in CAIStateGroupBase::GetPThingWorld() over local CFirst / CSecond models.
//   Ego_r.pdb 0x00847DF0 and FableWin.pdb 0x0288D960 ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ (protected const), the
//   declaration in engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE_PEEK) and seven landed callers that bind this
//   symbol at 0x008FCFA0 (reloc_bindings, first 0x0090FD20). Layout from the shared headers (CAIBrain::ThingManager +0x1c,
//   CThingManager::World +0x24, Ego_r).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingManager.h"

const CWorld& CAIStateGroupBase::PeekWorld() const
{
    return PBrain->ThingManager->PeekWorld();   // FableWin ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ, inline
}
