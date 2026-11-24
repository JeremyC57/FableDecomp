#ifndef FABLE_ENGINE_CJACKOFBLADESBATTLEDEF_H
#define FABLE_ENGINE_CJACKOFBLADESBATTLEDEF_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "fable_string.h"
#include "engine/CParentDefClassBase.h"
class CJackOfBladesBattleDef:public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26
 static CCharString FABLE_FASTCALL StatGetTypeName(){return CCharString("CJackOfBladesBattleDef",-1);}
#else
 static CCharString FABLE_FASTCALL StatGetTypeName();
#endif
 float IdealShootDistance;long NumGetHitsBeforeRush;
 float TimeBetweenShots,ScreamInAirTime,TimeBetweenPillars,BattleFloatHeight,BattleScale,ScaleUpTime,ScaleDownTime,DeathScreamTime,DeathHeight,BurnDamage,FlashDuration,FlashBuildUpTime,RushTriggerDistance,RushToDistance,RushAwayDistance;
#ifdef FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004E39E0: ?GetSizeofClass@CJackOfBladesBattleDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
FABLE_STATIC_ASSERT_AT(11, sizeof(CJackOfBladesBattleDef)==0x80);
#endif
