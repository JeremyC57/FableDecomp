// NScript::CQ_AmbushTradersScript::OnPersist(CPersistContext&) -- transfer the quest's persisted progress flags and counters with their original labels and defaults.
// Leaf: ?OnPersist@CQ_AmbushTradersScript@NScript@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_QUEST_SCRIPT_PERSIST
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CPersistContext.h"
void NScript::CQ_AmbushTradersScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestFailed",QuestFailed);
    context.Transfer("QuestFinished",QuestFinished);
    context.Transfer("QuestSucceeded",QuestSucceeded);
    context.Transfer("QuestOver",QuestOver);
    context.Transfer("AllBanditsDead",AllBanditsDead);
    context.Transfer("AllTradersDead",AllTradersDead);
    context.Transfer("AllGuardsDead",AllGuardsDead);
    context.Transfer("HeroDead",HeroDead);
    context.Transfer("SpyDead",SpyDead);
    context.Transfer("ContactDead",ContactDead);
    context.Transfer("ContactAttackedByHero",ContactAttackedByHero);
    context.Transfer("RegionInvalid",RegionInvalid);
    context.Transfer("BanditIntroDone",BanditIntroDone);
    context.Transfer("ConvoyTrigger2",ConvoyTrigger2);
    context.Transfer("TraderEscaped",TraderEscaped);
    context.Transfer("NoEnemiesLeft",NoEnemiesLeft);
    context.Transfer("PhaseComplete",PhaseComplete);
    context.Transfer("SentryAttackedByHero",SentryAttackedByHero);
    context.Transfer("SpyFollowing",SpyFollowing);
    context.Transfer("SpyEscaped",SpyEscaped);
    context.Transfer("SpyExited",SpyExited);
    context.Transfer("SpyHeroProximity",SpyHeroProximity);
    context.Transfer("SpyWanderReset",SpyWanderReset);
    context.Transfer("SpyContactMade",SpyContactMade);
    context.Transfer("AnyTraderEscaped",AnyTraderEscaped);
    context.Transfer("RegionClear",RegionClear);
    context.Transfer("QuestSuccessSpeechComplete",QuestSuccessSpeechComplete);
    context.Transfer("SpyFollowingHero",SpyFollowingHero);
    context.Transfer("SpySeenBySentry",SpySeenBySentry);
    context.Transfer("SpyReachedNoEntryMarker",SpyReachedNoEntryMarker);
    context.Transfer("TraderAliveCount",TraderAliveCount);
    context.Transfer("GuardAliveCount",GuardAliveCount);
    context.Transfer("ActivatedPhase3",ActivatedPhase3);
    context.Transfer("BanditContactKilledBeforeQuestEnd",BanditContactKilledBeforeQuestEnd);
    context.Transfer("SpeechBanditCount",SpeechBanditCount);
    context.Transfer("SpeechSpyCount",SpeechSpyCount);
    context.Transfer("SpeechSpyProximityCount",SpeechSpyProximityCount);
    context.Transfer("SpeechSpySentryCount",SpeechSpySentryCount);
    context.Transfer("HiredBanditCount",HiredBanditCount);
    context.Transfer("BanditAttackedByHero",BanditAttackedByHero);
    context.Transfer("BanditAttackByHeroCount",BanditAttackByHeroCount);
    context.Transfer("BanditSwitchAllegienceComplete",BanditSwitchAllegienceComplete);
}
