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
#if defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY) || defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY)
class CScriptGameResourceObjectScriptedThingBase;
#endif
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
    // Retail opt-in (chunk 797): the TeleportData record CAT_ConvoyMember::Main 0x00F0CAA0 builds and pushes onto TeleportInfo.
    // Ego_r.pdb CQ_AmbushTradersScript::TeleportData (32 bytes, struct key U: ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@XZ /
    // ??1..@@QAE@XZ / ??0..@@QAE@ABU012@@Z): defName +0, appearanceSeed +4, health +8, scriptName +0xc, itemsCarried +0x10 (the retail
    // STLport vector is 12 bytes: 0x1c). Retail bytes: the out-of-line ctor 0x00F0D470 (CCharString() at +0 / +0xc, zero words
    // +0x10..+0x18, +4 / +8 left unset) and dtor 0x00CDBC50 (the vector<CCharString> at +0x10, then +0xc and +0); Main writes
    // +4 (EntityGetAppearanceSeed), +0 (GetDefName), +8 (GetHealth), +0xc and +0x10. Both declared only (out of line in retail).
    struct TeleportData {
        TeleportData();                                                  // 0x00F0D470
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO
        // Retail opt-in (chunk 798): the copy ctor PhaseTwo 0x00F0C140 calls out of line for "TeleportData data = TeleportInfo[i];"
        // (Ego_r.pdb ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@ABU012@@Z; the COMDAT 0x00F0CA60 right after PhaseTwo's TU body).
        TeleportData(const TeleportData&);                               // 0x00F0CA60
#endif
        ~TeleportData();                                                 // 0x00CDBC50
        CCharString defName;                                             // +0x00
        unsigned long appearanceSeed;                                    // +0x04
        float health;                                                    // +0x08
        CCharString scriptName;                                          // +0x0c
        std::vector<CCharString> itemsCarried;                           // +0x10
    };
#else
    struct TeleportData;
#endif
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
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO
    // Retail opt-in (chunk 798): Ego_r.pdb ?PhaseTwo@CQ_AmbushTradersScript@NScript@@QAEXXZ (0x00F0C140), called by DoQuest at 0x00F0B691
    // after PhaseOne 0x00F0B780. Declarations only; no layout change.
    void PhaseTwo();                                                     // 0x00F0C140
#endif
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY
    // Retail opt-in (chunk 809): the parent helpers CAT_BanditAlly::Main 0x00F063B0 calls on ParentClass (Ego_r.pdb / FableWin.pdb
    // ?AddBanditHealthBar@CQ_AmbushTradersScript@NScript@@QAEJABVCScriptThing@@@Z, ?AskBanditHireQuestion@..@@QAE_NABVCScriptThing@@
    // AAVCScriptGameResourceObjectScriptedThingBase@@@Z, ?BanditHireSetup@..@@QAEJABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@@Z,
    // ?WatchForBanditSpeechMarkers@..@@QAEXVCScriptThing@@@Z, the spawned-thread member). Declarations only; no layout change.
    long AddBanditHealthBar(const CScriptThing&);                        // 0x00F07A80
    bool AskBanditHireQuestion(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&); // 0x00F07ED0
    long BanditHireSetup(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&);      // 0x00F081E0
    void WatchForBanditSpeechMarkers(CScriptThing);                      // 0x00F07B80
#endif
#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY
    // Retail opt-in (chunk 809): the parent helpers CAT_ConvoySpy::Main 0x00F0D630 calls on ParentClass (Ego_r.pdb / FableWin.pdb
    // ?RefreshSpyAllegiences@CQ_AmbushTradersScript@NScript@@QAEXXZ, ?WatchForSpyDeath@..@@QAEXVCScriptThing@@@Z,
    // ?WatchForHeroProximity@..@@QAEXVCScriptThing@@@Z, ?WatchForSpySpeechMarkers@..@@QAEXVCScriptThing@@@Z (the three spawned threads,
    // their member pointers stored at CSpawnedFunc1 +0x34), ?ProcessInitialSpyExtraction@..@@QAEXABVCScriptThing@@
    // AAVCScriptGameResourceObjectScriptedThingBase@@@Z). Declarations only; no layout change.
    void RefreshSpyAllegiences();                                        // 0x00F05A10
    void WatchForSpyDeath(CScriptThing);                                 // 0x00F0EDA0
    void WatchForHeroProximity(CScriptThing);                            // 0x00F0EE90
    void WatchForSpySpeechMarkers(CScriptThing);                         // 0x00F0EF60
    void ProcessInitialSpyExtraction(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&); // 0x00F0F680
#endif
};
}
#if defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER) && defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN)
// Retail opt-in (chunk 797): the nested entity script CAT_ConvoyMember (Ego_r.pdb, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18; the landed Alloc 0x00F04280 news 0x1c). Methods: Ego_r / FableWin ?Alloc@CAT_ConvoyMember@CQ_AmbushTradersScript@NScript@@SI...,
// ?Init@..@@EAEXXZ (0x00F08C30, landed), ?Main@..@@EAEXXZ (0x00F0CAA0; slot 1 of the entity vtable 0x012F7DF8).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript::CAT_ConvoyMember : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AmbushTradersScript&, CScriptDataBase*, const CScriptThing&); // 0x00F04280
    virtual CScriptBase* GetParentScript() const;                        // 0x00F04320
private:
    virtual void Main();                                                 // 0x00F0CAA0
    virtual void Init();                                                 // 0x00F08C30
protected:
    CQ_AmbushTradersScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript::TeleportData)==0x1c);
#endif
#if defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY) && defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN)
// Retail opt-in (chunk 809): the nested entity script CAT_BanditAlly (Ego_r.pdb, 44 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18, BanditHired +0x1c, BanditFollowing +0x1d, BanditTeleported +0x1e, BanditSwitchedAllegience +0x1f,
// BanditRemovedFromBowerstonePosh +0x20, ResetInvalidRegionTimer +0x21, BanditHealthBarID +0x24, InvalidRegionTimer +0x28; Main
// 0x00F063B0 reads/writes each at those offsets). Entity vtable 0x012F7D88 (stored by Alloc 0x00F037A0): 0 scalar deleting dtor
// 0x00F03870, 1 Main 0x00F063B0, 2 Init 0x00F05F30, 3 GetParentScript 0x00F03840, 4 OnPersist 0x00F0A8E0, 5 OnPredicateFail
// 0x00F03850. Names: Ego_r / FableWin ?Main@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ (private virtual), ?Init@..@@EAEXXZ,
// ?OnPersist@..@@EAEXAAVCPersistContext@@@Z, ?OnPredicateFail@..@@EAEXXZ, ?SwitchBanditAllegience@..@@AAEXXZ (private, 0x00F079D0).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript::CAT_BanditAlly : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AmbushTradersScript&, CScriptDataBase*, const CScriptThing&); // 0x00F037A0
    virtual CScriptBase* GetParentScript() const;                        // 0x00F03840
private:
    virtual void Main();                                                 // 0x00F063B0
    virtual void Init();                                                 // 0x00F05F30
    virtual void OnPersist(CPersistContext&);                            // 0x00F0A8E0
    virtual void OnPredicateFail();                                      // 0x00F03850
    void SwitchBanditAllegience();                                       // 0x00F079D0
protected:
    CQ_AmbushTradersScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool BanditHired;                                                    // +0x1c
    bool BanditFollowing;                                                // +0x1d
    bool BanditTeleported;                                               // +0x1e
    bool BanditSwitchedAllegience;                                       // +0x1f
    bool BanditRemovedFromBowerstonePosh;                                // +0x20
    bool ResetInvalidRegionTimer;                                        // +0x21
    long BanditHealthBarID;                                              // +0x24
    CTimer InvalidRegionTimer;                                           // +0x28
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript::CAT_BanditAlly)==0x2c);
#endif
#if defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY) && defined(FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN)
// Retail opt-in (chunk 809): the nested entity script CAT_ConvoySpy (Ego_r.pdb, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18). Entity vtable 0x012F7E14 (stored by Alloc 0x00F04360): 0 scalar deleting dtor 0x00F044F0, 1 Main 0x00F0D630,
// 2 Init 0x00F04410, 3 GetParentScript 0x00F04400, 4 OnPersist 0x00F04470, 5 OnPredicateFail 0x00F04480. Names: Ego_r / FableWin
// ?Main@CAT_ConvoySpy@CQ_AmbushTradersScript@NScript@@EAEXXZ (private virtual), ?Init@..@@EAEXXZ, ?OnPredicateFail@..@@EAEXXZ.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript::CAT_ConvoySpy : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AmbushTradersScript&, CScriptDataBase*, const CScriptThing&); // 0x00F04360
    virtual CScriptBase* GetParentScript() const;                        // 0x00F04400
private:
    virtual void Main();                                                 // 0x00F0D630
    virtual void Init();                                                 // 0x00F04410
    virtual void OnPersist(CPersistContext&);                            // 0x00F04470
    virtual void OnPredicateFail();                                      // 0x00F04480
protected:
    CQ_AmbushTradersScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript::CAT_ConvoySpy)==0x1c);
#endif
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript)==0x1d0);
#endif
