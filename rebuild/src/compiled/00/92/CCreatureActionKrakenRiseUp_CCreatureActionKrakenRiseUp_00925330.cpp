// CCreatureAction_KrakenRiseUp::CCreatureAction_KrakenRiseUp(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 0, zeroed CSoundPair) then SetAnimationName("RISE_FROM_SURFACE")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00925330, length 89 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// (the five-parameter base constructor), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("RISE_FROM_SURFACE"
// @ 0x012906D8, -1), CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0 (the manifest's
// "CTCInGameMenu::SetPDef" is a propagated label; every caller passes an animation name, see the landed
// 0x008C0520), CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x01290594 = `anonymous namespace'::CCreatureAction_KrakenRiseUp (rtti-vtables.tsv), so the manifest module CCombatAction_StabThing is a propagated label and wrong. Action code 0: VC reuses the pair's `xor eax,eax` for the pushed code and sinks the two zero stores below `push ecx ; push eax`.
// Shape: the vtable store is scheduled into the body between the string's argument pushes and its constructor
// call; the string is a named local so the setter argument is re-derived with `lea` (a bare temporary would
// reuse the constructor's eax). Engine header CCombatAction_StabThing.h is for the PDB build (base 0x12c v
// retail 0x114), so the layout is modelled locally.
#include "fable_string.h"

struct CThingCreatureBase;
struct CThing;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);                                                         // 0x008574D0
};

struct CCreatureAction_KrakenRiseUp : CCombatActionBase {
    virtual void v00();                         // own vtable 0x01290594
    CCreatureAction_KrakenRiseUp(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_KrakenRiseUp::CCreatureAction_KrakenRiseUp(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 0, CSoundPair())
{
    CCharString name("RISE_FROM_SURFACE", -1);
    SetAnimationName(name);
}