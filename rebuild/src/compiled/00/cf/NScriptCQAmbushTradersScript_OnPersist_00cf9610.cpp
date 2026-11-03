// NScript::CQ_AmbushTradersScript::OnPersist(CPersistContext&) -- transfer the quest's persisted progress flags and counters with their original labels and defaults.
// Leaf: ?OnPersist@CQ_AmbushTradersScript@NScript@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CPersistContext.h"
void NScript::CQ_AmbushTradersScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestFailed",QuestFailed,false);
    context.Transfer("QuestFinished",QuestFinished,false);
    context.Transfer("QuestSucceeded",QuestSucceeded,false);
    context.Transfer("QuestOver",QuestOver,false);
    context.Transfer("AllBanditsDead",AllBanditsDead,false);
    context.Transfer("AllTradersDead",AllTradersDead,false);
    context.Transfer("AllGuardsDead",AllGuardsDead,false);
    context.Transfer("HeroDead",HeroDead,false);
    context.Transfer("SpyDead",SpyDead,false);
    context.Transfer("ContactDead",ContactDead,false);
    context.Transfer("ContactAttackedByHero",ContactAttackedByHero,false);
    context.Transfer("RegionInvalid",RegionInvalid,false);
    context.Transfer("BanditIntroDone",BanditIntroDone,false);
    context.Transfer("ConvoyTrigger2",ConvoyTrigger2,false);
    context.Transfer("TraderEscaped",TraderEscaped,false);
    context.Transfer("NoEnemiesLeft",NoEnemiesLeft,false);
    context.Transfer("PhaseComplete",PhaseComplete,false);
    context.Transfer("SentryAttackedByHero",SentryAttackedByHero,false);
    context.Transfer("SpyFollowing",SpyFollowing,false);
    context.Transfer("SpyEscaped",SpyEscaped,false);
    context.Transfer("SpyExited",SpyExited,false);
    context.Transfer("SpyHeroProximity",SpyHeroProximity,false);
    context.Transfer("SpyWanderReset",SpyWanderReset,false);
    context.Transfer("SpyContactMade",SpyContactMade,false);
    context.Transfer("AnyTraderEscaped",AnyTraderEscaped,false);
    context.Transfer("RegionClear",RegionClear,false);
    context.Transfer("QuestSuccessSpeechComplete",QuestSuccessSpeechComplete,false);
    context.Transfer("SpyFollowingHero",SpyFollowingHero,false);
    context.Transfer("SpySeenBySentry",SpySeenBySentry,false);
    context.Transfer("SpyReachedNoEntryMarker",SpyReachedNoEntryMarker,false);
    context.Transfer("TraderAliveCount",TraderAliveCount,0L);
    context.Transfer("GuardAliveCount",GuardAliveCount,0L);
    context.Transfer("ActivatedPhase3",ActivatedPhase3,false);
    context.Transfer("BanditContactKilledBeforeQuestEnd",BanditContactKilledBeforeQuestEnd,false);
    context.Transfer("SpeechBanditCount",SpeechBanditCount,0L);
    context.Transfer("SpeechSpyCount",SpeechSpyCount,0L);
    context.Transfer("SpeechSpyProximityCount",SpeechSpyProximityCount,0L);
    context.Transfer("SpeechSpySentryCount",SpeechSpySentryCount,0L);
    context.Transfer("HiredBanditCount",HiredBanditCount,0L);
    context.Transfer("BanditAttackedByHero",BanditAttackedByHero,false);
    context.Transfer("BanditAttackByHeroCount",BanditAttackByHeroCount,0L);
    context.Transfer("BanditSwitchAllegienceComplete",BanditSwitchAllegienceComplete,false);
}