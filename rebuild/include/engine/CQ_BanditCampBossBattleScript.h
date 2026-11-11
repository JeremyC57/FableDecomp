#ifndef FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
#define FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
// Retail opt-in (chunk 783): NScript::CQ_BanditCampBossBattleScript, new header (no includer before chunk 783).
// Layout: Ego_r.pdb CQ_BanditCampBossBattleScript (struct_layouts_egor.tsv, 132 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): the flags +0x48..+0x54, KingHealth +0x58, flags +0x5c..+0x65, AngryBanditNeeded +0x68,
// BanditsNeededForCutscene +0x6c, PubGameChatted +0x6d -- the offsets the landed rows read (Init 0x00D03610 writes the flags,
// CCampHostage2::Main 0x00D08790 reads HostagesRescued +0x4e / HostageKilled +0x4f, CGate3Guard::Main 0x00D07490 reads
// Gate3Open +0x4d). Retail methods: ctor 0x00D035E0, Init 0x00D03610, RegisterMain 0x00D03820, WatchForTermination 0x00D04260.
// Nested entity script: only CGate3Guard so far (Ego_r.pdb CGate3Guard, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18; vtable 0x012C8E9C: 1 Main 0x00D07490, 2 Init 0x00D07410, 3 GetParentScript 0x00D073E0; Alloc 0x00D07340).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, as the Sunnyvale quest scripts (the master-data type reaches no mangled name
// used here). ParentClass is a reference as in the other quest headers (engine/CV_DemonDoorsScript.h, CQ_ArenaScript.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_BanditCampBossBattleScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D03820
    virtual void Main();
    virtual void Init();                                                 // 0x00D03610
    void WatchForTermination();                                          // 0x00D04260
    // Retail opt-in (chunk 787): the spawned thread bodies Main 0x00D038E0 hands to CSpawnedFunc (thread-name literals + Ego_r.pdb
    //   ?<Name>@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ), and the nested entity scripts its bindings allocate (Ego_r.pdb
    //   ?Alloc@<Class>@CQ_BanditCampBossBattleScript@NScript@@SI...); declarations only, no layout change.
    void BanditKingMissionProcess();                                     // 0x00D109F0
    void WatchForBanditCampGates();                                      // 0x00D04770
    void ProcessHostageCutscenes();                                      // 0x00D0FE80
    void CheckForFirstAreaMassacre();                                    // 0x00D0EE70
    void CheckForSecondAreaMassacre();                                   // 0x00D0F640
    void CheckForSecondAreaDoorHelp();                                   // 0x00D043A0
    class CGate2Guard1;                                                  // Alloc 0x00D071F0
    class CBCGameMaster;                                                 // Alloc 0x00D066B0
    class CAssassinMarker;                                               // Alloc 0x00D048C0
    class CAssassin1;                                                    // Alloc 0x00D04990
    class CAssassin2;                                                    // Alloc 0x00D05910
    class CAssassin3;                                                    // Alloc 0x00D05FE0
    class CCampHostage;                                                  // Alloc 0x00D07D20
    class CCampHostage2;                                                 // Alloc 0x00D08670
    class CCampHostageDoor;                                              // Alloc 0x00D08A70
    class CCampHostageGuard;                                             // Alloc 0x00D08E50
    class CBanditForger;                                                 // Alloc 0x00D0C2C0
    class CBanditKing;                                                   // Alloc 0x00D0A700
    class CCROWDBANDITS;                                                 // Alloc 0x00D0B2D0
    bool AttackedOuterGateGuards;                                        // +0x48
    bool AttackedGate2OuterGuard;                                        // +0x49
    bool AttackedGate3OuterGuard;                                        // +0x4a
    bool Gate1Open;                                                      // +0x4b
    bool Gate2Open;                                                      // +0x4c
    bool Gate3Open;                                                      // +0x4d
    bool HostagesRescued;                                                // +0x4e
    bool HostageKilled;                                                  // +0x4f
    bool ItsAllOver;                                                     // +0x50
    bool TheresaFrescoShown;                                             // +0x51
    bool BanditKingFightStarted;                                         // +0x52
    bool BanditKingFightEnded;                                           // +0x53
    bool BanditKingCrowdSpawn;                                           // +0x54
    long KingHealth;                                                     // +0x58 (PDB int)
    bool SpokenToSecondGuard;                                            // +0x5c
    bool MissionSucceeded;                                               // +0x5d
    bool MissionFailed;                                                  // +0x5e
    bool Gate1Passed;                                                    // +0x5f
    bool AssassinsUnderAttack;                                           // +0x60
    bool AssassinCutsceneTriggered;                                      // +0x61
    bool TalkedToAssassin;                                               // +0x62
    bool PlayGuardTooCloseCutscene;                                      // +0x63
    bool TwinBladeKilled;                                                // +0x64
    bool TwinBladeAttacked;                                              // +0x65
    long AngryBanditNeeded;                                              // +0x68
    bool BanditsNeededForCutscene;                                       // +0x6c
    bool PubGameChatted;                                                 // +0x6d
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GATE2GUARD1
    // Retail opt-in (Codex pass 21): both original PDBs, size 0x20; native
    // Alloc 00D071F0 and vtable 012C8E80 prove Main slot 1, Init slot 2.
    void OpenGate(float, CCharString); // 00D0EBC0, both PDB signatures and thread target.
    class CGate2Guard1 : public CEntityScriptBase {
    public:
        typedef CQ_BanditCampBossBattleScript TParentClass;
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Main();
        virtual void Init();
    protected:
        CQ_BanditCampBossBattleScript& ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData; // +0x18
    private:
        long AIState; // +0x1c
    };
#endif
    class CGate3Guard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D07340
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D073E0
    private:
        virtual void Main();                                             // slot 1 0x00D07490
        virtual void Init();                                             // slot 2 0x00D07410
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_BANDITKING
    // Retail opt-in (chunk 800): Ego_r.pdb CBanditKing (32 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18,
    //   HealthBarIndex +0x1c -- Main 0x00D0A830 stores AddQuestInfoBar's result at [esi+1Ch]); vtable 0x012C9474: 0 scalar deleting
    //   dtor 0x00D0A810, 1 Main 0x00D0A830, 2 CEntityScriptBase::Init 0x00CDEBB0 (not overridden), 3 GetParentScript 0x00D0A7A0,
    //   4 CEntityScriptBase::OnPersist 0x00CDEBC0, 5 OnPredicateFail 0x00D0A7B0, 6 CEntityScriptBase::OnInterrupted 0x00CDEBE0;
    //   Alloc 0x00D0A700 (the landed rows' leaves). Declarations only; existing includers do not define the macro.
    class CBanditKing : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D0A700
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D0A7A0
    private:
        virtual void Main();                                             // slot 1 0x00D0A830
        virtual void OnPredicateFail();                                  // slot 5 0x00D0A7B0
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        long HealthBarIndex;                                             // +0x1c
    };
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_HOSTAGE
    // Retail opt-in (chunk 852): Ego_r.pdb CCampHostage (28 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18);
    //   vtable 0x012C8F8C: 0 scalar deleting dtor 0x00D07E30, 1 Main 0x00D07E50, 2 Init 0x00D07DD0, 3 GetParentScript 0x00D07DC0,
    //   4..6 the CEntityScriptBase defaults 0x00CDEBC0 / 0x00CDEBD0 / 0x00CDEBE0; Alloc 0x00D07D20. Names: Ego_r.pdb / FableWin.pdb
    //   ?<Name>@CCampHostage@CQ_BanditCampBossBattleScript@NScript@@ (Main / Init private EAEXXZ). Declarations only.
    class CCampHostage : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D07D20
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D07DC0
    private:
        virtual void Main();                                             // slot 1 0x00D07E50
        virtual void Init();                                             // slot 2 0x00D07DD0
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GAMEMASTER
    // Retail opt-in (chunk 852): Ego_r.pdb CBCGameMaster (36 bytes = the retail new size in Alloc 0x00D066B0: CEntityScriptBase 0x14,
    //   ParentClass +0x14, PMasterData +0x18, AIState +0x1c, GivenPass +0x20 -- Init 0x00D06760 clears [ecx+20h], Main 0x00D067A0
    //   reads [esi+20h]); vtable 0x012C8DE4: 0 scalar deleting dtor 0x00D06780, 1 Main 0x00D067A0, 2 Init 0x00D06760,
    //   3 GetParentScript 0x00D06750, 4 OnPersist 0x00D0EE10, 5 OnPredicateFail 0x00D06770, 6 CEntityScriptBase::OnInterrupted
    //   0x00CDEBE0. Names: Ego_r.pdb / FableWin.pdb ?<Name>@CBCGameMaster@CQ_BanditCampBossBattleScript@NScript@@. Declarations only.
    class CBCGameMaster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D066B0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D06750
    private:
        virtual void Main();                                             // slot 1 0x00D067A0
        virtual void Init();                                             // slot 2 0x00D06760
        virtual void OnPersist(CPersistContext&);                        // slot 4 0x00D0EE10
        virtual void OnPredicateFail();                                  // slot 5 0x00D06770
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        long AIState;                                                    // +0x1c
        bool GivenPass;                                                  // +0x20
    };
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_FORGER
    // Retail opt-in (chunk 806): Ego_r.pdb CBanditForger (32 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18,
    //   GivenPass +0x1c -- Main 0x00D0C410 reads and sets [esi+1Ch]); vtable 0x012C9700: 0 scalar deleting dtor 0x00D0C370, 1 Main
    //   0x00D0C410, 2 Init 0x00D0C390, 3 GetParentScript 0x00D0C360, 4 OnPersist 0x00D0EE40; Alloc 0x00D0C2C0 (allocates 0x20 and
    //   stores the vtable). Names: Ego_r.pdb / FableWin.pdb ?<Name>@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@ (Main / Init
    //   private EAEXXZ). Declarations only; existing includers do not define the macro.
    class CBanditForger : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D0C2C0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D0C360
        virtual void OnPersist(CPersistContext&);                        // slot 4 0x00D0EE40
    private:
        virtual void Main();                                             // slot 1 0x00D0C410
        virtual void Init();                                             // slot 2 0x00D0C390
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        bool GivenPass;                                                  // +0x1c
    };
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CAMPHOSTAGEGUARD
    // Retail opt-in (chunk 812): Ego_r.pdb CCampHostageGuard (32 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18,
    //   DropPass +0x1c); vtable 0x012C9130 (stored by the landed Alloc 0x00D08E50): 0 scalar deleting dtor 0x00D08FF0, 1 Main 0x00D09010,
    //   2 Init 0x00D08F00, 3 GetParentScript 0x00D08EF0, 4 CEntityScriptBase::OnPersist 0x00CDEBC0, 5 OnPredicateFail 0x00D08FB0,
    //   6 CEntityScriptBase::OnInterrupted 0x00CDEBE0. Ego_r.pdb / FableWin.pdb ?Main@ / ?Init@ / ?OnPredicateFail@CCampHostageGuard@
    //   CQ_BanditCampBossBattleScript@NScript@@EAEXXZ (private). Declarations only; existing includers do not define the macro.
    class CCampHostageGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D08E50
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D08EF0
    private:
        virtual void Main();                                             // slot 1 0x00D09010
        virtual void Init();                                             // slot 2 0x00D08F00
        virtual void OnPredicateFail();                                  // slot 5 0x00D08FB0
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    private:
        bool DropPass;                                                   // +0x1c
    };
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript) == 0x70);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CGate3Guard) == 0x1c);
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_BANDITKING
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CBanditKing) == 0x20);
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_FORGER
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CBanditForger) == 0x20);
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_HOSTAGE
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CCampHostage) == 0x1c);
#endif
#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GAMEMASTER
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CBCGameMaster) == 0x24);
#endif

#ifdef FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CAMPHOSTAGEGUARD
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard) == 0x20);
#endif
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
