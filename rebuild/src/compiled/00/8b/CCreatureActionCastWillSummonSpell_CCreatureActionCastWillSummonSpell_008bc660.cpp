// CCreatureAction_CastWillSummonSpell::CCreatureAction_CastWillSummonSpell(CThingCreatureBase& creature, long levelToCast) -- CCombatActionBase(creature, 150, zeroed CSoundPair), LevelToCast, then the combat animation "ST_CAST_SUMMON_SPELL"
// Leaf: ??0CCreatureAction_CastWillSummonSpell@@QAE@AAVCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008bc660, 100 bytes, thiscall, `ret 8`.
// Identity (proven): it stores vtable 0x0127D704 (RTTI .?AVCCreatureAction_CastWillSummonSpell@@) after the CCombatActionBase constructor 0x00858030; Ego_r.pdb ??0CCreatureAction_CastWillSummonSpell@@QAE@AAVCThingCreatureBase@@J@Z; Ego_r.pdb member LevelToCast (the last field; retail +0x114); animation literal 0x0127D848 "ST_CAST_SUMMON_SPELL".
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030, CCharString::CCharString(
//   const char*, long) 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0, ~CCharString 0x0099EAE0.
// Shape (family L091): the family L076 constructor with the one-creature base constructor and the member initialiser stored after
//   the vtable (retail +0x114, the class's only own member).
// Header debt: engine/CCreatureAction_CastWillSummonSpell.h / CCombatActionBase.h are flat layouts without bases or methods; modelled locally.
#include "fable_string.h"
class CThingCreatureBase;
class CSoundPair {                                       // Ego_r.pdb, 8 bytes
public:
    long FirstSound;                                     // +0x00
    long LastSound;                                      // +0x04
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    CCombatActionBase(CThingCreatureBase& creature, long code, const CSoundPair& sounds);   // 0x00858030
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    unsigned char _pad04[0x110];                         // +0x04 (retail CCombatActionBase is 0x114 bytes)
};
class CCreatureAction_CastWillSummonSpell : public CCombatActionBase {
public:
    CCreatureAction_CastWillSummonSpell(CThingCreatureBase& creature, long value);
    virtual ~CCreatureAction_CastWillSummonSpell();
    long LevelToCast;                                          // +0x114
};
CCreatureAction_CastWillSummonSpell::CCreatureAction_CastWillSummonSpell(CThingCreatureBase& creature, long value)
    : CCombatActionBase(creature, 150, CSoundPair()), LevelToCast(value)
{
    {
        CCharString name("ST_CAST_SUMMON_SPELL", -1);
        SetCombatAnimation(name);
    }
}