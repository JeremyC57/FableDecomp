// CThingCreatureBase::ClearCurrentAction() -- release the PCurrentAction counted pointer (+0x100): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006633a0, 64 bytes, thiscall, plain `ret`.
// Identity (probable): Ego_r.pdb ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ; Ego_r.pdb CThingCreatureBase::PCurrentAction (CCountedPointer<CCreatureActionBase>, PDB +0x11C, retail +0x100 after the STLport shrink); callers CCreatureActionBase::Update 0x006929A0 (tail jump on the owner when the action is done), ::EditUpdate 0x00692DB0, CGameScriptInterface::SheatheHeroWeapons 0x008916A0 and the ScriptedThing ClearAllActions 0x00903970 (after the queued-action clears).
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L028): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
class CThingCreatureBase {
public:
    unsigned char _pad[0x100];                         // (members not touched here)
    CCountedPointer<CCreatureActionBase> PCurrentAction;                        // +0x100
    void ClearCurrentAction();
};
void CThingCreatureBase::ClearCurrentAction()
{
    PCurrentAction.ReleaseData();
}