// CCreatureAction_BuildUpBattleCharge::CCreatureAction_BuildUpBattleCharge(CThingCreatureBase& creature, CThing& target) -- CCombatActionBase(creature, target, 140, zeroed CSoundPair), then the combat animation "ST_BATTLE_CHARGE_BUILD_UP"
// Leaf: ??0CCreatureAction_BuildUpBattleCharge@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ce3f0, 95 bytes, thiscall, `ret 8`.
// Identity (proven): it stores vtable 0x012844E4 (RTTI .?AVCCreatureAction_BuildUpBattleCharge@@) after the CCombatActionBase constructor; Ego_r.pdb ??0CCreatureAction_BuildUpBattleCharge@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z; the animation literal at 0x01284628 is "ST_BATTLE_CHARGE_BUILD_UP"; the two-argument overload (Ego_r.pdb also has a one-argument one, ??0CCreatureAction_BuildUpBattleCharge@@QAE@AAVCThingCreatureBase@@@Z); no direct caller.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) 0x00857F30,
//   CCharString::CCharString(const char*, long) 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0,
//   CCharString::~CCharString 0x0099EAE0.
// Shape (family L076): the landed 0x00908000 constructor -- the CSoundPair temporary zeroed in the frame by its inline
//   constructor, the vtable stored after the base constructor, the name a scoped named CCharString local.
// Header debt: engine/CCreatureAction_BuildUpBattleCharge.h / CCombatActionBase.h are flat byte layouts without bases or methods; modelled locally.
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
class CCreatureAction_BuildUpBattleCharge : public CCombatActionBase {
public:
    CCreatureAction_BuildUpBattleCharge(CThingCreatureBase& creature, CThing& target);
    virtual ~CCreatureAction_BuildUpBattleCharge();
};
CCreatureAction_BuildUpBattleCharge::CCreatureAction_BuildUpBattleCharge(CThingCreatureBase& creature, CThing& target)
    : CCombatActionBase(creature, target, 140, CSoundPair())
{
    {
        CCharString name("ST_BATTLE_CHARGE_BUILD_UP", -1);
        SetCombatAnimation(name);
    }
}