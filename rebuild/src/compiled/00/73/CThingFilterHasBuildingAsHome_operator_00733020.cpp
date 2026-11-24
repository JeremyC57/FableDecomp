// NEntityEvents::CIsHitEventHitBy::operator()(const CEventHitBase* event) const -- whether the hit event was dealt by the functor's thing
// Leaf: ??RCIsHitEventHitBy@NEntityEvents@@QBE_NPBVCEventHitBase@1@@Z
// Name fix (NF382, QUEUE q6): was CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature&), binding 0x00810FC0 as
//   CThingAICreature::GetHomeBuilding. 0x00810FC0 is NEntityEvents::CEventHitBase::GetPHitBy: its retail callers are the
//   CGameScriptThing::MsgWhoHitMe / MsgIsHitBy* family and the CTCEntityEvents::GetEvent<CEventHitBy...> instances, exactly the
//   FableWin callers of GetPHitBy (game_script_thing.obj, tc_combat.obj ...), and engine/NEntityEvents_CEventHitBase.h declares
//   it there. Of the three FableWin functors that call GetPHitBy only CIsHitEventHitBy::operator() (tc_combat.obj, 41 bytes /Od)
//   is a bare `GetPHitBy() == HitBy` compare (CIsHitEventHitAndKnockedDownBy adds IsKnockDown, ...ThingWithScriptName compares
//   names); this address sits just before CCombatAbility::IsActive 0x00733090, the start of the retail tc_combat.obj block.
//   Ego_r's real CThingFilter_HasBuildingAsHome reads the creature's own +0x190 pointer (thing_search_filters.obj). The function
//   is unreferenced in retail (an emitted COMDAT, /OPT:NOREF). Same bytes.
// Pick-any form (NF387, T3 nf382 0x0067C490 line): operator() is defined in the class, so it is an inline member and its COMDAT is
//   pick-any (Any), like the copies the landed GetEvent TUs emit; HEAD's out-of-line definition was NoDuplicates (a duplicate at link
//   beside them). Same body, same bytes.
// Grade: RELOCATION_MATCH -- the call result is held in a named local and compared `hitBy == HitBy` (gives retail `cmp eax,edx`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733020, length 27 bytes.
// Callees: NEntityEvents::CEventHitBase::GetPHitBy() const @ 0x00810FC0 (`mov ecx,[ecx+8] ; add ecx,50h ; jmp GetPItem`).
// Layout: Ego_r.pdb CIsHitEventHitBy {CThing* HitBy +0} (4 bytes).
// Shape: `return event->GetPHitBy() == HitBy;` through a full register (`xor ecx,ecx ; cmp eax,edx ; sete cl ; mov al,cl`),
// the member loaded after the call (`mov edx,[esi]`).
// Local model: engine/NEntityEvents_CEventHitBase.h declares the same GetPHitBy at 0x00810FC0, but its CHitParameters /
// CIntelligentPointer chain needs the full def/thing opt-in set; the callee spelling here is identical.
class CThing;
namespace NEntityEvents {
class CEventHitBase { public:
    CThing* GetPHitBy() const;                  // 0x00810FC0
};
class CIsHitEventHitBy { public:
    CThing* HitBy;                              // +0x00
    bool operator()(const CEventHitBase* event) const   // class-inline: VC7.1 emits it as a pick-any COMDAT (selection Any)
    {
        const CThing* hitBy = event->GetPHitBy();
        return hitBy == HitBy;
    }
};
}   // namespace NEntityEvents

namespace {
// Fidelity: scaffolding, not retail: the address of the class-inline operator(), so this TU emits it out of line as a pick-any COMDAT,
// as retail's tc_combat TU does (unreferenced, /OPT:NOREF) and the landed GetEvent TUs do (0x00738A80 carries the same Any copy).
// Internal to this TU.
typedef bool (NEntityEvents::CIsHitEventHitBy::*TIsHitEventHitByCall)(const NEntityEvents::CEventHitBase*) const;
TIsHitEventHitByCall ReferenceIsHitEventHitByCall()
{
    return &NEntityEvents::CIsHitEventHitBy::operator();
}
}
