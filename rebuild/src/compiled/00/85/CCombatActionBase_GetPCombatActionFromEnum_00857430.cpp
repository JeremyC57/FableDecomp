// CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature, CThingCreatureBase& target) -- clone the registered prototype for the type as a combat action, give it its creature and target and return it counted; a null counted pointer when the type is not registered, its prototype is null or the clone fails
// Leaf: ?GetPCombatActionFromEnum@CCombatActionBase@@SI?AV?$CCountedPointer@VCCombatActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@1@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857430, length 148 bytes (+12 int3). Static `__fastcall`, `ret 8` (the hidden
//   CCountedPointer result in ecx, the type in edx, creature and target on the stack).
// Callees: the action registry's find(const EActionRegisteredType&) @ 0x00696680 (iterator through a hidden pointer),
//   the prototype's virtual Clone() const (slot 11, +0x2C), CCreatureActionBase::SetCreature @ 0x006928C0,
//   CCombatActionBase::SetTarget(CThing&) @ 0x0085AA90 (Ego_r.pdb names it; the landed body stores the target into the
//   intelligent pointer at +0xA8; manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is a propagated twin),
//   CCountedPointer<CCombatActionBase>::CCountedPointer(CCombatActionBase*) @ 0x0085A840 (landed).
// Data: the registry std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > @ 0x013BA8C8 shared with
//   CCreatureActionBase::GetPActionFromEnum (landed 0x00692E50, the one-argument sibling this body extends): header pointer +0,
//   node value pair +0x10, so the prototype's Data is node +0x14.
// Shape: the end test and the null-prototype test are two separate early returns, which VC merges into one null block on
//   the fall-through of `jne` (one `||` condition moves that block to the end of the function); the clone's null test keeps
//   the success path on the fall-through with a second null return (after `pop edi`) at the end.
// Name: the catalog row already says GetPCombatActionFromEnum; the Ego_r.pdb symbol above has the two creature references.
// NF52 (round 7): engine/CCreatureActionBase.h with the static RegisteredActions (FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS) over the
// shared STLport map (stlport/fable_map.h; find 0x00696680 is the tree's find), SetCreature by the overlay opt-in
// FABLE_RETAIL_CCREATUREACTIONBASE_SETCREATURE; CCombatActionBase from engine/CCombatActionBase.h (GETPCOMBATACTIONFROMENUM, and the overlay
// opt-in FABLE_RETAIL_CCOMBATACTIONBASE_SETTARGET for SetTarget 0x0085AA90). Were local std::pair / std::map / CCreatureActionBase /
// CCombatActionBase / CThing / CThingCreatureBase models and the made-up Registry.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETCREATURE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_compiler.h"
#include "engine/CCreatureActionBase.h"

#define FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETTARGET
#include "engine/CCombatActionBase.h"

CCountedPointer<CCombatActionBase> __fastcall CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType type,
                                                                                          CThingCreatureBase& creature,
                                                                                          CThingCreatureBase& target)
{
    std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = RegisteredActions.find(type);
    if (it == RegisteredActions.end())
        return CCountedPointer<CCombatActionBase>();
    if (!it->second.Data)
        return CCountedPointer<CCombatActionBase>();
    CCombatActionBase* action = (CCombatActionBase*)it->second->Clone();
    if (action) {
        action->SetCreature(creature);
        action->SetTarget((CThing&)target);
        return CCountedPointer<CCombatActionBase>(action);
    }
    return CCountedPointer<CCombatActionBase>();
}
