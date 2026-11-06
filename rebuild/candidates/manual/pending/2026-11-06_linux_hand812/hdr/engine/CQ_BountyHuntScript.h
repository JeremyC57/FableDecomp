#ifndef FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H
#define FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H
// Original Ego_r TPI 0x1317c, field order and types; native base is 0x48.
// Layout: native Game+0x40; QuestFailed+0x48; BoastTime+0x80.
// Chunk 797: opt-in FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2 (TeleportData ctor/dtor, WatchForDeputy2GruntsKilled, CBH_GangDeputy2).
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BountyHuntScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    struct TeleportData {
        CCharString defName; unsigned long appearanceSeed; float health;
        CCharString scriptName, teleportTo, teleportMkr;
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
        TeleportData();                                                  // 0x00D15140
        ~TeleportData();                                                 // 0x00CD4550
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAIN
        // Retail opt-in (chunk 812): Main 0x00D13580 calls the ctor and the copy out of line (Ego_r.pdb ??0TeleportData@ / ??4TeleportData@
        // CQ_BountyHuntScript@NScript@@QAE...) and keeps the dtor implicit (one exit expands it, the other calls the COMDAT 0x00CD4550).
#ifndef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
        TeleportData();                                                  // 0x00D15140
#endif
        TeleportData& operator=(const TeleportData&);                    // 0x00D15170
#endif
    };
    CQ_BountyHuntScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_BountyHuntScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void SpawnBanditGangGrunts(CCharString spawnName);
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
    void WatchForDeputy2GruntsKilled();                                  // 0x00D19AF0
    class CBH_GangDeputy2;                                               // Alloc 0x00D161A0
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAIN
    // Retail opt-in (chunk 812): the threads Main 0x00D13580 spawns (functor stores; Ego_r.pdb + FableWin.pdb
    // ?Watch...@CQ_BountyHuntScript@NScript@@QAEXXZ) and the entity scripts it binds (Ego_r.pdb ?Alloc@CBH_...@CQ_BountyHuntScript@NScript@@SI...).
    void WatchForQuestFailure();                                         // 0x00D15090
    void WatchForQuestSuccess();                                         // 0x00D150F0
    void WatchForEnableFisherCreekHutDoor();                             // 0x00D151D0
    void WatchForAllGruntsKilled();                                      // 0x00D17340
    void WatchForBoasts();                                               // 0x00D17A80
    class CBH_GangGruntSpawn;                                            // Alloc 0x00D153F0
    class CBH_GangGrunt;                                                 // Alloc 0x00D15940
    class CBH_GangDeputy1;                                               // Alloc 0x00D15FC0
    class CBH_Hostage1;                                                  // Alloc 0x00D160B0
    class CBH_Hostage2;                                                  // Alloc 0x00D16290
#ifndef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGLEADER
    class CBH_GangLeader;                                                // Alloc 0x00D16380
#endif
#ifndef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
    class CBH_GangDeputy2;                                               // Alloc 0x00D161A0
#endif
#endif
    bool QuestFailed;
    bool QuestSucceeded;
    bool QuestOver;
    bool QuestStarted;
    bool FishermanDeactivated;
    bool VillagerKilled;
    bool VillagerIntroDone;
    bool LeaderKilled;
    bool Deputy1Killed;
    bool Deputy2Killed;
    bool Deputy1IntroComplete;
    bool Deputy2IntroComplete;
    bool Hostage1Killed;
    bool Hostage2Killed;
    bool Hostage1Safe;
    bool Hostage2Safe;
    bool Deputy1Escaped;
    bool LeaderEscaped;
    bool SpawnDeputy1Grunts;
    bool SpawnDeputy2Grunts;
    bool SpawnLeaderGrunts;
    bool AllDeputy1GruntsKilled;
    bool AllDeputy2GruntsKilled;
    bool AllLeaderGruntsKilled;
    bool AllLeaderChargeGruntsKilled;
    bool AllGruntsKilled;
    bool FishComplete;
    bool LeaderIntro1Complete;
    bool LeaderIntro2Complete;
    bool LeaderAttackingHostage;
    bool LeaderAttackingHero;
    bool LeaderRansomPaid;
    bool LeaderRansomDenied;
    bool LeaderLeftHostage;
    bool LeaderReachedMoney;
    bool LeaderAttackedAfterRansomPaid;
    bool LeaderChargeComplete;
    bool LeaderCutsceneComplete;
    bool LakeComplete;
    bool BanditCutsceneDone;
    bool BountyHuntIntroComplete;
    long BanditGruntTextCounter;
    bool BoastActiveTimeLimit;
    bool BoastActiveDecapitation;
    long BoastTimerID;
    CTimer BoastTime;
    bool BoastQuestInfoDisplayed;
    std::vector<TeleportData> TeleportInfo;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript)==0x94);
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
// Opt-in (chunk 797): the CBH_GangDeputy2 entity script (Ego_r.pdb CBH_GangDeputy2, 28 bytes: ParentClass +0x14, PMasterData +0x18)
// and the TeleportData ctor / dtor its Main 0x00D18DF0 calls out of line (Ego_r.pdb ??0 / ??1TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ),
// plus WatchForDeputy2GruntsKilled (landed 0x00D19AF0, the thread Main spawns). Entity vtable stored by the landed Alloc 0x00D161A0:
// Main 0x00D18DF0, Init 0x00D169E0, OnPersist 0x00D16250, OnPredicateFail 0x00D16260, GetParentScript 0x00D16240.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript::CBH_GangDeputy2 : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); // 0x00D161A0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D16240
private:
    virtual void Main();                                                 // 0x00D18DF0
    virtual void Init();                                                 // 0x00D169E0
    virtual void OnPersist(CPersistContext&);                            // 0x00D16250
    virtual void OnPredicateFail();                                      // 0x00D16260
protected:
    CQ_BountyHuntScript& ParentClass;                                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript::CBH_GangDeputy2) == 0x1c);
#endif
#endif
