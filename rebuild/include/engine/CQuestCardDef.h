#ifndef FABLE_ENGINE_CQUESTCARDDEF_H
#define FABLE_ENGINE_CQUESTCARDDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
// Both original PDB enum records: int, sole enumerator zero.
enum EQuestCardPrerequisite { QUEST_PREREQUISITE_RENOWN = 0 };
// Layout: both original complete PDB classes, native005e5118/005e5e63;
// original vector contracts20 to12; native RewardObjects+4c,QuestEpilogue+70.
class CQuestCardDef:public CSubDefClassBase {
public:
 long QuestName,QuestSummary,QuestObjective,SuccessSummary;
 CDefString RegionName,TeleporterRegionName;
 long InventoryCategory,RenownReward,GoldReward;
 std::vector<long> RewardObjects;
 bool IsCore,IsVignette,IsExclusive,MakeVignetteRouteAppearOnMinimap;
 std::map<EQuestCardPrerequisite,long> Prerequisites;
 long NumBoasts;
 bool CanPlayerCancel;
 unsigned long QuestEpilogue;
};
FABLE_STATIC_ASSERT(sizeof(CQuestCardDef)==0x74);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef,RewardObjects)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CQuestCardDef,QuestEpilogue)==0x70);
#endif
