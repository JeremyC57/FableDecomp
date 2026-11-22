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
};
FABLE_STATIC_ASSERT_AT(11, sizeof(CJackOfBladesBattleDef)==0x80);
#endif
