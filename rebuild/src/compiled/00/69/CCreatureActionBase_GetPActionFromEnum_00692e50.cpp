// CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature) -- clone the registered prototype action for the type, give it the creature and return it counted; a null counted pointer when the type is not registered
// Leaf: ?GetPActionFromEnum@CCreatureActionBase@@SI?AV?$CCountedPointer@VCCreatureActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
//
// Retail 0x00692E50, 105 bytes (+ int3 padding to 112), static __fastcall `ret 4` (the hidden CCountedPointer result
//   in ecx, the type in edx, the creature on the stack).
// Callees: the registry map's find(const EActionRegisteredType&) @ 0x00696680 (landed; iterator through a hidden
//   pointer, `ret 8`), the prototype's virtual Clone() const (slot 11, +0x2C), CCreatureActionBase::SetCreature @
//   0x006928C0 (landed as the intelligent-pointer assign to PCreature +0x08), CCountedPointer<CCreatureActionBase>::
//   CCountedPointer(CCreatureActionBase*) @ 0x00695880 (landed).
// Data: the registry std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > @ 0x013BA8C8 (header
//   pointer +0, node count +4; its static initialiser 0x0121E070 and atexit destructor 0x012292E0 are landed); a
//   node's value pair sits at +0x10, so the prototype's Data is node +0x14.
// Caller: 0x0066770E.
// Shape: `it = Registry.find(type); if (it == Registry.end()) return CCountedPointer<CCreatureActionBase>();
//   CCreatureActionBase* action = it->second->Clone(); action->SetCreature(creature); return
//   CCountedPointer<CCreatureActionBase>(action);` -- the type is homed in a frame slot for find's reference, the
//   empty pointer is the inline default ctor (two zero stores), the full one constructed in the result slot.
// Name: Ego_r.pdb `?GetPActionFromEnum@CCreatureActionBase@@SI?AV?$CCountedPointer@VCCreatureActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@@Z`
//   (exact symbol); the catalog's "GetPCombatActionFromEnum" is a near-miss label.  The registry's own name is not in
//   the PDB name sets (descriptive "Registry").
// NF52 (round 7): engine/CCreatureActionBase.h with the static RegisteredActions (FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS)
// over the shared STLport map (stlport/fable_map.h); SetCreature by overlay opt-in.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETCREATURE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_compiler.h"
#include "engine/CCreatureActionBase.h"

CCountedPointer<CCreatureActionBase> __fastcall CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType type,
                                                                                        CThingCreatureBase& creature)
{
    std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = RegisteredActions.find(type);
    if (it == RegisteredActions.end())
        return CCountedPointer<CCreatureActionBase>();
    CCreatureActionBase* action = it->second->Clone();
    action->SetCreature(creature);
    return CCountedPointer<CCreatureActionBase>(action);
}
