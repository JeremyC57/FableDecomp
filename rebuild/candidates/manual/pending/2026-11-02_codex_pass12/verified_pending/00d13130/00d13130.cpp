// NScript::CQ_BountyHuntScript::OnPersist(CPersistContext&) -- transfer the quest's persisted progress flags and grunt text counter with their original labels and defaults.
// Leaf: ?OnPersist@CQ_BountyHuntScript@NScript@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_QUEST_SCRIPT_PERSIST
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CPersistContext.h"
void NScript::CQ_BountyHuntScript::OnPersist(CPersistContext& cp)
{
    cp.Transfer("QuestFailed",QuestFailed);
    cp.Transfer("QuestSucceeded",QuestSucceeded);
    cp.Transfer("QuestOver",QuestOver);
    cp.Transfer("QuestStarted",QuestStarted);
    cp.Transfer("FishermanDeactivated",FishermanDeactivated);
    cp.Transfer("VillagerKilled",VillagerKilled);
    cp.Transfer("VillagerIntroDone",VillagerIntroDone);
    cp.Transfer("LeaderKilled",LeaderKilled);
    cp.Transfer("Deputy1Killed",Deputy1Killed);
    cp.Transfer("Deputy2Killed",Deputy2Killed);
    cp.Transfer("Hostage1Killed",Hostage1Killed);
    cp.Transfer("Hostage2Killed",Hostage2Killed);
    cp.Transfer("Hostage1Safe",Hostage1Safe);
    cp.Transfer("Hostage2Safe",Hostage2Safe);
    cp.Transfer("Deputy1Escaped",Deputy1Escaped);
    cp.Transfer("LeaderEscaped",LeaderEscaped);
    cp.Transfer("Deputy1IntroComplete",Deputy1IntroComplete);
    cp.Transfer("Deputy2IntroComplete",Deputy2IntroComplete);
    cp.Transfer("LeaderIntro1Complete",LeaderIntro1Complete);
    cp.Transfer("LeaderIntro2Complete",LeaderIntro2Complete);
    cp.Transfer("LeaderAttackingHostage",LeaderAttackingHostage);
    cp.Transfer("LeaderAttackingHero",LeaderAttackingHero);
    cp.Transfer("LeaderRansomPaid",LeaderRansomPaid);
    cp.Transfer("LeaderRansomDenied",LeaderRansomDenied);
    cp.Transfer("LeaderLeftHostage",LeaderLeftHostage);
    cp.Transfer("LeaderReachedMoney",LeaderReachedMoney);
    cp.Transfer("LeaderAttackedAfterRansomPaid",LeaderAttackedAfterRansomPaid);
    cp.Transfer("LeaderChargeComplete",LeaderChargeComplete);
    cp.Transfer("LeaderCutsceneComplete",LeaderCutsceneComplete);
    cp.Transfer("SpawnDeputy1Grunts",SpawnDeputy1Grunts);
    cp.Transfer("SpawnDeputy2Grunts",SpawnDeputy2Grunts);
    cp.Transfer("SpawnLeaderGrunts",SpawnLeaderGrunts);
    cp.Transfer("AllDeputy1GruntsKilled",AllDeputy1GruntsKilled);
    cp.Transfer("AllDeputy2GruntsKilled",AllDeputy2GruntsKilled);
    cp.Transfer("AllLeaderGruntsKilled",AllLeaderGruntsKilled);
    cp.Transfer("AllLeaderChargeGruntsKilled",AllLeaderChargeGruntsKilled);
    cp.Transfer("AllGruntsKilled",AllGruntsKilled);
    cp.Transfer("BanditCutsceneDone",BanditCutsceneDone);
    cp.Transfer("FishComplete",FishComplete);
    cp.Transfer("LakeComplete",LakeComplete);
    cp.Transfer("BountyHuntIntroComplete",BountyHuntIntroComplete);
    cp.Transfer("BanditGruntTextCounter",BanditGruntTextCounter);
    cp.Transfer("BoastQuestInfoDisplayed",BoastQuestInfoDisplayed);
}
