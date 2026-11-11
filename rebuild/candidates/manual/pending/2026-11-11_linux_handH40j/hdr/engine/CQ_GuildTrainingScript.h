#ifndef FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGSCRIPT_H
// Original Ego_r TPI110ab/110ca. Native CWillDummy factory00d43360/vtable012ce364.
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain(); virtual void Main(); virtual void Init(); virtual void OnPersist(CPersistContext&);
    long GameState;
    bool SleepNeeded;
    bool ReachedPlatform;
    bool DomeCutsceneStart;
    long HeroWarnings;
    long CurrentBirdsKilled;
    long StartedTesting;
    bool StartedMeleeTesting;
    CTimer WillHelpTimer;
    bool FightFinished;
    bool DisplayBirdKilledMessage;
#ifdef FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_INIT
    // Retail opt-in (chunk 858): Init 0x00D3B3D0 (vtable 0x012CD458 slot 3) ends in a tail jump to 0x00D3B410, the landed
    //   Ego_r.pdb / FableWin.pdb ?GossipSetup@CQ_GuildTrainingScript@NScript@@QAEXXZ (public, non-virtual); declaration only.
    void GossipSetup();                                                  // 0x00D3B410
#endif
#ifdef FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
    // Retail opt-in (chunk 800): the members Main 0x00D3BC60 calls (Ego_r.pdb ?<Name>@CQ_GuildTrainingScript@NScript@@QAEXXZ, the
    //   CSpawnedFunc thread-name literals "RunTutorials" .. "KeepBookcaseExitRemoved" next to each member pointer) and the nested
    //   entity scripts its bindings allocate (Ego_r.pdb ?Alloc@<Class>@CQ_GuildTrainingScript@NScript@@SI..., binding-name literals);
    //   declarations only, no layout change (vtable 0x012CD458: 0 dtor 0x00D50650, 1 RegisterMain 0x00D3BB50, 2 Main 0x00D3BC60,
    //   3 Init 0x00D3B3D0, 4 OnPersist 0x00D3BC10).
    void RunArrivalCutscene();                                           // 0x00D44CB0
    void RunTutorials();                                                 // 0x00D45DD0
    void CheckFriendlyAttacks();                                         // 0x00D45060
    void KeepTabsOnWhisper();                                            // 0x00D3CBD0
    void WatchForSparrowKilled();                                        // 0x00D3C840
    void KeepBookcaseExitRemoved();                                      // 0x00D3C8F0
    class CAppleGirl;                                                    // Alloc 0x00D3D060
    class CApprenticeSpeedTest;                                          // Alloc 0x00D3E1F0
    class CSpeedFriend;                                                  // Alloc 0x00D407C0
    class CRaceMarker;                                                   // Alloc 0x00D409E0
    class CMeleeApprentice;                                              // Alloc 0x00D40C10
    class CCombatApprentice;                                             // Alloc 0x00D41A30
    class CSkillApprentice;                                              // Alloc 0x00D41B10
    class CWillApprentice;                                               // Alloc 0x00D43280
    class CHeroBed;                                                      // Alloc 0x00D40B30
    class CSkillTarget;                                                  // Alloc 0x00D41BF0
    class CBirdKiller;                                                   // Alloc 0x00D42F40
    class CKillBird;                                                     // Alloc 0x00D43040
    class CPreMeleeMaze;                                                 // Alloc 0x00D43CD0
#endif
    class CWillDummy : public CEntityScriptBase {
    public: virtual CScriptBase* GetParentScript() const;
#ifdef FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D43360 (chunk 800)
#endif
    protected: CQ_GuildTrainingScript& ParentClass; CQ_SunnyvaleMasterData* PMasterData;
    private: virtual void Main(); virtual void Init(); virtual void OnPredicateFail();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript)==0x68);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CWillDummy)==0x1c);
#if defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES) && defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN)
// Retail opt-in (chunk 805): two entity scripts (needs FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN for the nested declarations).
// Ego_r.pdb layouts (struct_layouts_egor.tsv; entity offsets equal retail's, CEntityScriptBase 0x14 in both): CAppleGirl 40 bytes
// (AppleMode +0x1c, CurrentApples +0x20, ChildAppleMode +0x24, HaveChatted +0x25), CSkillTarget 40 bytes (Speed +0x1c,
// DummyNumber +0x20, DummyWorth +0x24); the landed Allocs 0x00D3D060 / 0x00D41BF0 allocate 0x28. Entity vtables 0x012CD7E8
// (CAppleGirl: 0 scalar deleting dtor 0x00D3D130, 1 Main 0x00D3D150, 2 Init 0x00D3D110, 3 GetParentScript 0x00D3D100) and
// 0x012CE0CC (CSkillTarget: 0 0x00D41CE0, 1 Main 0x00D41D00, 2 Init 0x00D41CA0, 3 GetParentScript 0x00D41C90); Main / Init are
// private (PDB ?Main@<Class>@CQ_GuildTrainingScript@NScript@@EAEXXZ).
namespace NScript {
class CQ_GuildTrainingScript::CAppleGirl : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&); // 0x00D3D060
    virtual CScriptBase* GetParentScript() const;                        // 0x00D3D100
private:
    virtual void Main();                                                 // 0x00D3D150
    virtual void Init();                                                 // 0x00D3D110
protected:
    CQ_GuildTrainingScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long AppleMode;                                                      // +0x1c
    long CurrentApples;                                                  // +0x20
    bool ChildAppleMode;                                                 // +0x24
    bool HaveChatted;                                                    // +0x25
};
class CQ_GuildTrainingScript::CSkillTarget : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&); // 0x00D41BF0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D41C90
private:
    virtual void Main();                                                 // 0x00D41D00
    virtual void Init();                                                 // 0x00D41CA0
protected:
    CQ_GuildTrainingScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long Speed;                                                          // +0x1c
    long DummyNumber;                                                    // +0x20
    long DummyWorth;                                                     // +0x24
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CAppleGirl) == 0x28);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CSkillTarget) == 0x28);
#endif
#if defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDTEST) && defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN)
// Retail opt-in (chunk 816): CApprenticeSpeedTest, Ego_r.pdb layout (struct_layouts_egor.tsv, 32 bytes: ParentClass +0x14,
// PMasterData +0x18, RaceMode +0x1c; the landed Alloc 0x00D3E1F0 allocates 0x20). Entity vtable 0x012CDA40: 0 scalar deleting dtor
// 0x00D3E2C0, 1 Main 0x00D3E2E0, 2 Init 0x00D3E2A0, 3 GetParentScript 0x00D3E290, 4 OnPredicateFail 0x00D3E2B0 (Ego_r.pdb / FableWin.pdb
// ?Main@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXXZ).
namespace NScript {
class CQ_GuildTrainingScript::CApprenticeSpeedTest : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&); // 0x00D3E1F0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D3E290
private:
    virtual void Main();                                                 // 0x00D3E2E0
    virtual void Init();                                                 // 0x00D3E2A0
    virtual void OnPredicateFail();                                      // 0x00D3E2B0
protected:
    CQ_GuildTrainingScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    long RaceMode;                                                       // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest) == 0x20);
#endif
#if defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MELEEAPPRENTICE) && defined(FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN)
// Retail opt-in (chunk 842): the melee apprentice entity script (needs FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN for the nested
// declaration). Ego_r.pdb layout (struct_layouts_egor.tsv; entity offsets equal retail's): CMeleeApprentice 40 bytes, ParentClass
// +0x14, PMasterData +0x18, WaitingForFight +0x1c, BadHit +0x20, WillWoodsChatDone +0x24; the landed Alloc 0x00D40C10 allocates 0x28.
// Entity vtable 0x012CDF7C: 0 scalar deleting dtor 0x00D40CD0, 1 Main 0x00D40CF0, 2 Init 0x00D40CC0, 3 GetParentScript 0x00D40CB0,
// 4-6 the CEntityScriptBase defaults (dw dump); Main / Init private (PDB ?Main@CMeleeApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ).
namespace NScript {
class CQ_GuildTrainingScript::CMeleeApprentice : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&); // 0x00D40C10
    virtual CScriptBase* GetParentScript() const;                        // 0x00D40CB0
private:
    virtual void Main();                                                 // 0x00D40CF0
    virtual void Init();                                                 // 0x00D40CC0
protected:
    CQ_GuildTrainingScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool WaitingForFight;                                                // +0x1c
    long BadHit;                                                         // +0x20
    bool WillWoodsChatDone;                                              // +0x24
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingScript::CMeleeApprentice) == 0x28);
#endif
#endif
