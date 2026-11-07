#ifndef FABLE_ENGINE_CQ_HANGINGTREEEVILSCRIPT_H
#define FABLE_ENGINE_CQ_HANGINGTREEEVILSCRIPT_H
// NScript::CQ_HangingTreeEvilScript (chunk 744): Ego_r.pdb field order and names (struct_layouts_egor.tsv, 0xF0 bytes), retail
// offsets = PDB - 0x14 (retail CLevelScriptBase is 0x48) with 12-byte STLport vectors: retail size 0xD0 (factory 0x00D6ED10
// `push 0D0h`; ctor 0x00D68F00 builds ReasonForFailing +0x5c, the four CScriptThing members, MissionTimer +0xa4, CurrentLevel
// +0xa8 and the three vectors +0xac/+0xb8/+0xc4). Vtable 0x012D1F98: ~ 0x00D6ED40, RegisterMain 0x00D69390, Main 0x00D69480,
// Init 0x00D68FF0, OnPersist 0x00D69450. Retail fields read by the landed rows: MissionSucceeded +0x4c, MissionFailed +0x4d
// (Main 0x00D69480), HTE_State +0x60, MissionTimer +0xa4 (DoMission 0x00D6BFA0).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> as the sibling CQ_HangingTreeGoodScript (Game +0x40, MasterScriptData +0x44,
// stored by the ctor); the master-data type is the Good script's (CQ_SunnyvaleMasterData carries HangingTreeBanditKilled /
// HangingTreeGuardKilled) -- the argument type does not reach any mangled name used here.
// Nested entity scripts: the eight bound by Main (FableWin/Ego_r ?Alloc@CHTE_*@CQ_HangingTreeEvilScript@NScript@@SI...), with
// their retail Alloc addresses (the binding's ClassAllocFunc stores in Main 0x00D69480).
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HangingTreeEvilScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00D68F00
    virtual void RegisterMain();                                         // slot 1 0x00D69390
    virtual void Main();                                                 // slot 2 0x00D69480
    virtual void Init();                                                 // slot 3 0x00D68FF0
    virtual void OnPersist(CPersistContext&);                            // slot 4 0x00D69450
    void BanditYells();                                                  // 0x00D69CD0
    void CleanUp();                                                      // 0x00D6B720
    void DoMission();                                                    // 0x00D6BFA0
#ifdef FABLE_RETAIL_CQ_HANGINGTREEEVILSCRIPT_INTRODUCTION
    // Retail opt-in (chunk 831, row 0x00D6D080): Ego_r.pdb / FableWin.pdb ?Introduction@CQ_HangingTreeEvilScript@NScript@@QAEXXZ;
    // the body plays CS_HANGINGTREE_EVIL_INTRO (0x012D2400) and DoMission 0x00D6BFA0 calls it after SpawnParty (0x00D6C1F8).
    void Introduction();                                                 // 0x00D6D080
#endif
    bool IntroCutscenePlayed;                                            // +0x48
    bool BanditsStarted;                                                 // +0x49
    bool BanditKilled;                                                   // +0x4a
    bool PartySuccessfullySetUp;                                         // +0x4b
    bool MissionSucceeded;                                               // +0x4c
    bool MissionFailed;                                                  // +0x4d
    bool BanditsLeftRegion;                                              // +0x4e
    bool SpawnHere;                                                      // +0x4f
    bool AtWayPoint;                                                     // +0x50
    bool SwitchingRegions;                                               // +0x51
    bool RemoveBandits;                                                  // +0x52
    bool AllEnemiesDead;                                                 // +0x53
    bool QuestStartScreenShown;                                          // +0x54
    bool BanditYelledRecently;                                           // +0x55
    bool ExecutionerInterrupted;                                         // +0x56
    bool AudienceLeggit;                                                 // +0x57
    bool ShouldFollowHero;                                               // +0x58
    CCharString ReasonForFailing;                                        // +0x5c
    long HTE_State;                                                      // +0x60
    long CurrentBandits;                                                 // +0x64
    long BanditsAtNextRegion;                                            // +0x68
    long TimerGUIIndex;                                                  // +0x6c
    long CarryOverGuards;                                                // +0x70
    CScriptThing Executioner;                                            // +0x74
    CScriptThing Prisoner;                                               // +0x80
    CScriptThing PartySpawnPoint;                                        // +0x8c
    CScriptThing BanditExitPoint;                                        // +0x98
    CTimer MissionTimer;                                                 // +0xa4
    CCharString CurrentLevel;                                            // +0xa8
    std::vector<CScriptThing,std::allocator<CScriptThing> > CurrentEnemyVector;    // +0xac
    std::vector<CScriptThing,std::allocator<CScriptThing> > CurrentBanditVector;   // +0xb8
    std::vector<CCharString,std::allocator<CCharString> > AreasSpawnedIn;          // +0xc4
    class CHTE_AmbushPoint : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D69D50
    class CHTE_AssaultGuard : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D6AD70
    // Retail layout (chunk 781): Ego_r.pdb CHTE_FreedomFighter (struct_layouts_egor.tsv, 0x20 bytes: ParentClass +0x14,
    // PMasterData +0x18, HeroHitMe +0x1c; factory 0x00D6A3D0 `push 20h`); vtable 0x012D2170 [~ 0x00D6A540, Main 0x00D6A560,
    // Init 0x00D6A480, GetParentScript 0x00D6A470, 0x00CDEBC0, OnPredicateFail 0x00D6A4F0, 0x00CDEBE0]; the private virtuals
    // are FableWin/Ego_r ?Main/?Init/?OnPredicateFail@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ.
    class CHTE_FreedomFighter : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&);   // Alloc 0x00D6A3D0
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D6A470
        CQ_HangingTreeEvilScript* ParentClass;                           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        long HeroHitMe;                                                  // +0x1c
    private:
        virtual void Main();                                             // slot 1 0x00D6A560
        virtual void Init();                                             // slot 2 0x00D6A480
        virtual void OnPredicateFail();                                  // slot 5 0x00D6A4F0
    };
    class CHTE_Audience : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D6B180
    class CHTE_Prisoner : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D69E30
    class CHTE_Executioner : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D69FD0
    class CGUARD1 : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D6A150
    class CGUARD2 : public CEntityScriptBase { public: static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HangingTreeEvilScript&,CScriptDataBase*,const CScriptThing&); };   // Alloc 0x00D6A290
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HangingTreeEvilScript)==0xd0);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter)==0x20);
#endif
