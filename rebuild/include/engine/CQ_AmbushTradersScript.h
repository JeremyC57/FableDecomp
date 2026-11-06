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
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
    // Retail opt-in (chunk 788): the members Main 0x00F024C0 (slot 2 of the script vtable 0x012C6CAC; slot 3 is the landed Init
    // 0x00CF9A50) spawns and binds; no layout change. Threads: Ego_r.pdb ?<Name>@CQ_AmbushTradersScript@NScript@@QAEXXZ, each paired in
    // Main with its thread-name literal. Entity scripts: Ego_r.pdb ?Alloc@<Class>@CQ_AmbushTradersScript@NScript@@SI... (binding-name
    // literal = class name less its `C`), forward-declared only.
    void WatchForQuestFailure();                                         // 0x00F03270
    void WatchForQuestSuccess();                                         // 0x00F0A470
    void WatchForBoastSuccess();                                         // 0x00F03560
    void WatchForHeroDeath();                                            // 0x00F032C0
    void WatchForAllGuardsDead();                                        // 0x00F033F0
    void WatchForAllTradersDead();                                       // 0x00F03330
    void WatchForBanditSpeechTimer();                                    // 0x00F035E0
    void WatchForSpySpeechTimer();                                       // 0x00F036C0
    void WatchForRegionChange();                                         // 0x00F058D0
    void DoQuest();                                                      // 0x00F0B130
    void WatchForRegionClear();                                          // 0x00F09CD0
    class CAT_BanditAlly;                                                // Alloc 0x00F037A0
    class CAT_BanditContact;                                             // Alloc 0x00F057E0
    class CAT_BanditContactEnd;                                          // Alloc 0x00F056A0
    class CAT_Phase1Guard;                                               // Alloc 0x00F038A0
    class CAT_Phase1Archer;                                              // Alloc 0x00F03F20
    class CAT_Phase3Guard;                                               // Alloc 0x00F050E0
    class CAT_Phase3Archer;                                              // Alloc 0x00F04BD0
    class CAT_ConvoyMember;                                              // Alloc 0x00F04280
    class CAT_ConvoySpy;                                                 // Alloc 0x00F04360
    class CAT_Sentry;                                                    // Alloc 0x00F04510
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript)==0x1d0);
#endif
