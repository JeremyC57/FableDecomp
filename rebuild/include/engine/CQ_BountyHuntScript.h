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
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS
    void WatchForBoasts();                                               // 0x00D17A80 (Ego_r.pdb QAEXXZ)
    void WatchForQuestInfoDisplayConditions();                           // 0x00D17DA0 (Ego_r.pdb QAEXXZ)
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY1
    void WatchForDeputy1GruntsKilled();                                  // 0x00D18B20 (retail; Ego_r.pdb QAEXXZ)
    class CBH_GangDeputy1;                                               // Alloc 0x00D15FC0
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE1
    class CBH_Hostage1;                                                  // Alloc 0x00D160B0
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE2
    class CBH_Hostage2;                                                  // Alloc 0x00D16290
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGLEADER // Retail opt-in (Codex pass 21): original PDB/native leader owner and watchers; rebased hand810 model.
    void WatchForLeaderGruntsKilled();                                   // 0x00D1C9E0
    void WatchForLeaderChargeGruntsKilled();                             // 0x00D1CAC0
    class CBH_GangLeader;                                                // Alloc 0x00D16380
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
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY1
// Retail opt-in (chunk 833): the CBH_GangDeputy1 entity script (Ego_r.pdb CBH_GangDeputy1, 28 bytes = the landed Alloc 0x00D15FC0's
// new size: ParentClass +0x14, PMasterData +0x18) and WatchForDeputy1GruntsKilled (Ego_r.pdb ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ),
// the thread its Main 0x00D17F20 spawns (member pointer 0x00D18B20 stored at CSpawnedFunc +0x34). Main is slot 1 of the entity vtable
// 0x012CA554 the landed Alloc stores; GetParentScript 0x00D16060 / OnPersist 0x00D16070 / OnPredicateFail 0x00D16080 are landed.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript::CBH_GangDeputy1 : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); // 0x00D15FC0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D16060
private:
    virtual void Main();                                                 // 0x00D17F20
    virtual void Init();                                                 // 0x00D16460
    virtual void OnPersist(CPersistContext&);                            // 0x00D16070
    virtual void OnPredicateFail();                                      // 0x00D16080
protected:
    CQ_BountyHuntScript& ParentClass;                                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript::CBH_GangDeputy1) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE1
// Retail opt-in (chunk 833): the CBH_Hostage1 entity script (Ego_r.pdb CBH_Hostage1, 28 bytes = the landed Alloc 0x00D160B0's new size:
// ParentClass +0x14, PMasterData +0x18). Entity vtable 0x012CA570 (stored by the Alloc): 0 scalar deleting dtor 0x00D16180, 1 Main 0x00D19BD0,
// 2 Init 0x00D167C0, 3 GetParentScript 0x00D16150, 4 OnPersist 0x00D16160, 5 OnPredicateFail 0x00D16170 (read from retail .rdata).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript::CBH_Hostage1 : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); // 0x00D160B0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D16150
private:
    virtual void Main();                                                 // 0x00D19BD0
    virtual void Init();                                                 // 0x00D167C0
    virtual void OnPersist(CPersistContext&);                            // 0x00D16160
    virtual void OnPredicateFail();                                      // 0x00D16170
protected:
    CQ_BountyHuntScript& ParentClass;                                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript::CBH_Hostage1) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_HOSTAGE2
// Retail opt-in (chunk 833): the CBH_Hostage2 entity script (Ego_r.pdb CBH_Hostage2, 28 bytes = the landed Alloc 0x00D16290's new size:
// ParentClass +0x14, PMasterData +0x18). Entity vtable 0x012CA5A8 (stored by the Alloc): 0 scalar deleting dtor 0x00D16360, 1 Main 0x00D1A740,
// 2 Init 0x00D16BB0, 3 GetParentScript 0x00D16330, 4 OnPersist 0x00D16340, 5 OnPredicateFail 0x00D16350 (read from retail .rdata).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript::CBH_Hostage2 : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); // 0x00D16290
    virtual CScriptBase* GetParentScript() const;                        // 0x00D16330
private:
    virtual void Main();                                                 // 0x00D1A740
    virtual void Init();                                                 // 0x00D16BB0
    virtual void OnPersist(CPersistContext&);                            // 0x00D16340
    virtual void OnPredicateFail();                                      // 0x00D16350
protected:
    CQ_BountyHuntScript& ParentClass;                                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript::CBH_Hostage2) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGLEADER // Retail opt-in (Codex pass 21): original PDB/native leader owner and watchers; rebased hand810 model.
// Opt-in (chunk 810): the CBH_GangLeader entity script (Ego_r.pdb CBH_GangLeader, 44 bytes: ParentClass +0x14, PMasterData +0x18,
// hitCount +0x1c, stage1Done..stage4Done +0x20..+0x23, HealthInitial +0x24, HealthCurrent +0x28) and the two watch threads its
// Main 0x00D1B200 spawns (Ego_r.pdb + FableWin.pdb ?WatchForLeaderGruntsKilled@ / ?WatchForLeaderChargeGruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ;
// functor stores 0x00D1B955 / 0x00D1BEB9, thread names "WatchForLeaderGruntsKilled" / "WatchForLeaderChargeGruntsKilled").
// Entity vtable 0x012CA5C4 stored by the landed Alloc 0x00D16380: Main 0x00D1B200, Init 0x00D16DC0, GetParentScript 0x00D16420,
// OnPersist 0x00D179B0, OnPredicateFail 0x00D16430.
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_BountyHuntScript::CBH_GangLeader : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); // 0x00D16380
    virtual CScriptBase* GetParentScript() const;                        // 0x00D16420
private:
    virtual void Main();                                                 // 0x00D1B200
    virtual void Init();                                                 // 0x00D16DC0
    virtual void OnPersist(CPersistContext&);                            // 0x00D179B0
    virtual void OnPredicateFail();                                      // 0x00D16430
protected:
    CQ_BountyHuntScript& ParentClass;                                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    long hitCount;                                                       // +0x1c
    bool stage1Done;                                                     // +0x20
    bool stage2Done;                                                     // +0x21
    bool stage3Done;                                                     // +0x22
    bool stage4Done;                                                     // +0x23
    float HealthInitial;                                                 // +0x24
    float HealthCurrent;                                                 // +0x28
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript::CBH_GangLeader) == 0x2c);
#endif

#endif
