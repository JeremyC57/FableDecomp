// CCreatureAction_TrollIdle::CCreatureAction_TrollIdle(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 50, zeroed CSoundPair) then SetAnimationName("SPECIAL_IDLE")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00908270, length 89 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30
// (the five-parameter base constructor), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SPECIAL_IDLE"
// @ 0x0127A43C, -1), CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0 (the manifest's
// "CTCInGameMenu::SetPDef" is a propagated label; every caller passes an animation name, see the landed
// 0x008C0520), CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x0128E79C = CCreatureAction_TrollIdle (rtti-vtables.tsv), so the manifest module CCombatAction_StabThing is a propagated label and wrong. Action code 50; the pair's zero stores precede the pushes.
// Shape: the vtable store is scheduled into the body between the string's argument pushes and its constructor
// call; the string is a named local so the setter argument is re-derived with `lea` (a bare temporary would
// reuse the constructor's eax). Engine header CCombatAction_StabThing.h is for the PDB build (base 0x12c v
// retail 0x114), so the layout is modelled locally.
#include "fable_string.h"

class CThingCreatureBase;
class CThing;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_TrollIdle : public CCombatActionBase { public:
    virtual void v00();                         // own vtable 0x0128E79C
    CCreatureAction_TrollIdle(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_TrollIdle::CCreatureAction_TrollIdle(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 50, CSoundPair())
{
    CCharString name("SPECIAL_IDLE", -1);
    SetCombatAnimation(name);
}