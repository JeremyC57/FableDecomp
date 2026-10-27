// CCreatureAction_TrollGroundPound::CCreatureAction_TrollGroundPound(CThingCreatureBase& creature, CThingCreatureBase& target) -- CCombatActionBase(creature, target, 150, zeroed CSoundPair), then the combat animation "GROUND_POUND"
// Leaf: ??0CCreatureAction_TrollGroundPound@@QAE@AAVCThingCreatureBase@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094e5f0, 95 bytes, thiscall, `ret 8`.
// Identity (proven): it stores vtable 0x012931CC (RTTI .?AVCCreatureAction_TrollGroundPound@@) after the CCombatActionBase constructor; Ego_r.pdb ??0CCreatureAction_TrollGroundPound@@QAE@AAVCThingCreatureBase@@0@Z; the animation literal at 0x01293310 is "GROUND_POUND"; callers CAIStateGroup_TrollBase::ThrowRocks 0x0094DC40, the rock / earth troll Update bodies 0x0094DD90 / 0x0094E7B0 and CAIStateGroup_IceTrollPoundGroundAttack::Update 0x009624D0; label correction: reloc_bindings names it ??0CCombatAction_StabThing@@QAE@PAVCThingCreatureBase@@PAVCThing@@@Z, but the vtable it stores is CCreatureAction_TrollGroundPound's.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) 0x00857F30,
//   CCharString::CCharString(const char*, long) 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0,
//   CCharString::~CCharString 0x0099EAE0.
// Shape (family L076): the landed 0x00908000 constructor -- the CSoundPair temporary zeroed in the frame by its inline
//   constructor, the vtable stored after the base constructor, the name a scoped named CCharString local.
// Header debt: engine/CCreatureAction_TrollGroundPound.h / CCombatActionBase.h are flat byte layouts without bases or methods; modelled locally.
#include "fable_string.h"
class CThing { };
class CThingCreatureBase : public CThing { };
class CSoundPair {                                       // Ego_r.pdb, 8 bytes
public:
    long FirstSound;                                     // +0x00
    long LastSound;                                      // +0x04
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    CCombatActionBase(CThingCreatureBase& creature, CThing& target, long code, const CSoundPair& sounds);   // 0x00857F30
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_TrollGroundPound : public CCombatActionBase {
public:
    CCreatureAction_TrollGroundPound(CThingCreatureBase& creature, CThingCreatureBase& target);
    virtual ~CCreatureAction_TrollGroundPound();
};
CCreatureAction_TrollGroundPound::CCreatureAction_TrollGroundPound(CThingCreatureBase& creature, CThingCreatureBase& target)
    : CCombatActionBase(creature, target, 150, CSoundPair())
{
    {
        CCharString name("GROUND_POUND", -1);
        SetCombatAnimation(name);
    }
}