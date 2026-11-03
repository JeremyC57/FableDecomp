// NScript::CQ_BountyHuntScript::OnPersist(CPersistContext&) -- transfer the quest's persisted progress flags and grunt text counter with their original labels and defaults.
// Leaf: ?OnPersist@CQ_BountyHuntScript@NScript@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CPersistContext.h"
void NScript::CQ_BountyHuntScript::OnPersist(CPersistContext& cp)
{
    cp.Transfer("QuestFailed",QuestFailed,false);
    cp.Transfer("QuestSucceeded",QuestSucceeded,false);
    cp.Transfer("QuestOver",QuestOver,false);
    cp.Transfer("QuestStarted",QuestStarted,false);
    cp.Transfer("FishermanDeactivated",FishermanDeactivated,false);
    cp.Transfer("VillagerKilled",VillagerKilled,false);
    cp.Transfer("VillagerIntroDone",VillagerIntroDone,false);
    cp.Transfer("LeaderKilled",LeaderKilled,false);
    cp.Transfer("Deputy1Killed",Deputy1Killed,false);
    cp.Transfer("Deputy2Killed",Deputy2Killed,false);
    cp.Transfer("Hostage1Killed",Hostage1Killed,false);
    cp.Transfer("Hostage2Killed",Hostage2Killed,false);
    cp.Transfer("Hostage1Safe",Hostage1Safe,false);
    cp.Transfer("Hostage2Safe",Hostage2Safe,false);
    cp.Transfer("Deputy1Escaped",Deputy1Escaped,false);
    cp.Transfer("LeaderEscaped",LeaderEscaped,false);
    cp.Transfer("Deputy1IntroComplete",Deputy1IntroComplete,false);
    cp.Transfer("Deputy2IntroComplete",Deputy2IntroComplete,false);
    cp.Transfer("LeaderIntro1Complete",LeaderIntro1Complete,false);
    cp.Transfer("LeaderIntro2Complete",LeaderIntro2Complete,false);
    cp.Transfer("LeaderAttackingHostage",LeaderAttackingHostage,false);
    cp.Transfer("LeaderAttackingHero",LeaderAttackingHero,false);
    cp.Transfer("LeaderRansomPaid",LeaderRansomPaid,false);
    cp.Transfer("LeaderRansomDenied",LeaderRansomDenied,false);
    cp.Transfer("LeaderLeftHostage",LeaderLeftHostage,false);
    cp.Transfer("LeaderReachedMoney",LeaderReachedMoney,false);
    cp.Transfer("LeaderAttackedAfterRansomPaid",LeaderAttackedAfterRansomPaid,false);
    cp.Transfer("LeaderChargeComplete",LeaderChargeComplete,false);
    cp.Transfer("LeaderCutsceneComplete",LeaderCutsceneComplete,false);
    cp.Transfer("SpawnDeputy1Grunts",SpawnDeputy1Grunts,false);
    cp.Transfer("SpawnDeputy2Grunts",SpawnDeputy2Grunts,false);
    cp.Transfer("SpawnLeaderGrunts",SpawnLeaderGrunts,false);
    cp.Transfer("AllDeputy1GruntsKilled",AllDeputy1GruntsKilled,false);
    cp.Transfer("AllDeputy2GruntsKilled",AllDeputy2GruntsKilled,false);
    cp.Transfer("AllLeaderGruntsKilled",AllLeaderGruntsKilled,false);
    cp.Transfer("AllLeaderChargeGruntsKilled",AllLeaderChargeGruntsKilled,false);
    cp.Transfer("AllGruntsKilled",AllGruntsKilled,false);
    cp.Transfer("BanditCutsceneDone",BanditCutsceneDone,false);
    cp.Transfer("FishComplete",FishComplete,false);
    cp.Transfer("LakeComplete",LakeComplete,false);
    cp.Transfer("BountyHuntIntroComplete",BountyHuntIntroComplete,false);
    cp.Transfer("BanditGruntTextCounter",BanditGruntTextCounter,0L);
    cp.Transfer("BoastQuestInfoDisplayed",BoastQuestInfoDisplayed,false);
}