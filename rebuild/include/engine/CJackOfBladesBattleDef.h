#ifndef FABLE_ENGINE_CJACKOFBLADESBATTLEDEF_H
#define FABLE_ENGINE_CJACKOFBLADESBATTLEDEF_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "engine/CParentDefClassBase.h"
class CJackOfBladesBattleDef:public CParentDefClassBase {
public:
 static CCharString FABLE_FASTCALL StatGetTypeName();
 float IdealShootDistance;long NumGetHitsBeforeRush;
 float TimeBetweenShots,ScreamInAirTime,TimeBetweenPillars,BattleFloatHeight,BattleScale,ScaleUpTime,ScaleDownTime,DeathScreamTime,DeathHeight,BurnDamage,FlashDuration,FlashBuildUpTime,RushTriggerDistance,RushToDistance,RushAwayDistance;
};
FABLE_STATIC_ASSERT(sizeof(CJackOfBladesBattleDef)==0x80);
#endif
