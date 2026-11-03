#ifndef FABLE_ENGINE_CQ_AMBUSHTRADERSSCRIPT_H
#define FABLE_ENGINE_CQ_AMBUSHTRADERSSCRIPT_H
// Original Ego_r TPI 0x1142e; FableWin independently confirms all fields.
// Layout: native base0x48, QuestFailed+0x48, six vectors+0xa8, CSFlagMap+0x1b8, TeleportInfo+0x1c4; total0x1d0.
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_map.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    struct TeleportData;
    CQ_AmbushTradersScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_AmbushTradersScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    bool QuestFailed;
    bool QuestFinished;
    bool QuestSucceeded;
    bool QuestOver;
    bool RegionInvalid;
    bool BanditIntroDone;
    bool TraderEscaped;
    bool NoEnemiesLeft;
    bool PhaseComplete;
    bool ConvoyTrigger2;
    bool AllBanditsDead;
    bool AllTradersDead;
    bool AllGuardsDead;
    bool ContactDead;
    bool ContactAttackedByHero;
    bool HeroDead;
    bool SpyDead;
    bool SentryAttackedByHero;
    bool SpySeenBySentry;
    bool SpyFollowing;
    bool SpyFollowingHero;
    bool SpyHeroProximity;
    bool SpyWanderReset;
    bool SpyContactMade;
    bool SpyEscaped;
    bool SpyExited;
    bool SpyReachedNoEntryMarker;
    bool AnyTraderEscaped;
    bool RegionClear;
    bool QuestSuccessSpeechComplete;
    bool ActivatedPhase3;
    bool BanditContactIntroComplete;
    bool BanditContactKilledBeforeQuestEnd;
    bool EnableSpeechSpy;
    bool EnableSpeechBandit;
    long TraderAliveCount;
    long GuardAliveCount;
    long SpeechBanditCount;
    long SpeechSpyCount;
    long SpeechSpyProximityCount;
    long SpeechSpySentryCount;
    long SpyHealthBarID;
    unsigned long ContactSeed;
    long HiredBanditCount;
    bool BanditAttackedByHero;
    long BanditAttackByHeroCount;
    bool BanditSwitchAllegienceComplete;
    CTimer SpeechBanditTimer;
    CTimer SpeechSpyTimer;
    long lastRandomSpySpeechIdx;
    std::vector<CScriptThing> AllBandits;
    std::vector<CScriptThing> AllTraders;
    std::vector<CScriptThing> AllGuards;
    std::vector<CScriptThing> BanditsVector;
    std::vector<CScriptThing> ConvoyMembersVector;
    std::vector<CScriptThing> SentriesVector;
    CCharString BanditSpeechNormalThere[5];
    CCharString BanditSpeechRandomThere[5];
    CCharString BanditSpeechNormalBack[5];
    CCharString BanditSpeechRandomBack[5];
    CCharString SpySpeechNormal[5];
    CCharString SpySpeechRandom[5];
    CCharString SpySpeechProximityNormal[5];
    CCharString SpySpeechProximityRandom[5];
    CCharString SpySpeechSentryNormal[5];
    CCharString SpySpeechSentryRandom[5];
    std::map<CCharString,bool> CSFlagMap;
    std::vector<TeleportData> TeleportInfo;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript)==0x1d0);
#endif
