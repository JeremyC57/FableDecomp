#ifndef FABLE_ENGINE_CTROLLBATTLEDEF_H
#define FABLE_ENGINE_CTROLLBATTLEDEF_H
#include "engine/CParentDefClassBase.h"
class CTrollBattleDef : public CParentDefClassBase {
public:
 static CCharString FABLE_FASTCALL StatGetTypeName(){return CCharString("CTrollBattleDef");}
 float GroundPoundSpeedMultiplierStart,GroundPoundSpeedMultiplierEnd;
 float AttackPosPredictiveness,PercentageChanceToUseSecondaryShotType;
 long SecondaryShotType;
 float MinLaughDistance,MaxTimeBetweenKnockdownAndBoast;
 long DeathExplosionIndex,FallDownExplosionIndex;
};
#endif
