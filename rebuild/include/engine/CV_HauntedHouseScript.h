#ifndef FABLE_ENGINE_CV_HAUNTEDHOUSESCRIPT_H
#define FABLE_ENGINE_CV_HAUNTEDHOUSESCRIPT_H
// Retail opt-in (chunk 808): NScript::CV_HauntedHouseScript, new header (no includer before chunk 808; the landed sibling rows keep
// their local models).
// Layout: Ego_r.pdb CV_HauntedHouseScript (struct_layouts_egor.tsv, 112 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): UndeadBar +0x48, GuiCounter +0x4C, CellarDoorOpen +0x50, HeroBeenToChest +0x51, QuestCardGiven +0x52,
// InHouse +0x53, UndeadAlive +0x54, MaxUndead +0x58, sizeof 0x5C (the landed Alloc 0x00E96DD0 news 0x5C; Main 0x00E93EB0 reads
// InHouse [esi+0x53] and CellarDoorOpen [esi+0x50]; the landed CheckRegionLeaving 0x00E953E0 reads GuiCounter +0x4C).
// Script vtable 0x012ECB94: 0 ??_G 0x00E96E10, 1 RegisterMain 0x00E93DA0, 2 Main 0x00E93EB0, 3 Init 0x00E93D80, 4 OnPersist 0x00E93E60.
// Nested entity scripts (Ego_r.pdb / FableWin.pdb ?Alloc@<C>@CV_HauntedHouseScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@...):
// CM_UndeadHere Alloc 0x00E95690, CHHUndead 0x00E95CD0, CM_QuestControl 0x00E955B0, CM_HeroInsideHouse 0x00E95BF0 (all landed).
// Methods: CheckRegionLeaving 0x00E953E0 (?CheckRegionLeaving@CV_HauntedHouseScript@NScript@@QAEXXZ, the "CheckRegionLeaving" thread).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CScriptDataBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E93DA0 (slot 1)
    virtual void Main();                                                 // 0x00E93EB0 (slot 2)
    virtual void Init();                                                 // 0x00E93D80 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00E93E60 (slot 4)
    void CheckRegionLeaving();                                           // 0x00E953E0
#ifdef FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
    // Add-only opt-in (NF336, 2026-11-24; local-model debt named by acctB Gate 0): the PDB members the landed level-script rows
    // define or call. Declarations only. Ego_r.pdb + FableWin.pdb: ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@
    // AAVCGameScriptInterfaceBase@@@Z (0x00E96DD0, stored by RegisterScripts 0x00CD8B2F) and
    // ?SendBodyguardsToTheRightPlace@CV_HauntedHouseScript@NScript@@QAEXVCCharString@@0@Z (0x00E96C30); FableWin.pdb
    // ??0CV_HauntedHouseScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (0x00E93D50, stores the vtable 0x012ECB94).
    // Ego_r.pdb (release) has no ??1: the destructor is implicit (FABLE_RETAIL_CLEVELSCRIPTBASE, ??_G 0x00E96E10 calls ~CScriptBase).
    CV_HauntedHouseScript(CScriptDataBase*, CGameScriptInterfaceBase&);  // 0x00E93D50
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&); // 0x00E96DD0
    void SendBodyguardsToTheRightPlace(CCharString, CCharString);       // 0x00E96C30
#endif
    float UndeadBar;                                                     // +0x48
    long GuiCounter;                                                     // +0x4C
    bool CellarDoorOpen;                                                 // +0x50
    bool HeroBeenToChest;                                                // +0x51
    bool QuestCardGiven;                                                 // +0x52
    bool InHouse;                                                        // +0x53
    long UndeadAlive;                                                    // +0x54
    long MaxUndead;                                                      // +0x58
    class CM_UndeadHere : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E95690
#ifdef FABLE_RETAIL_CM_UNDEADHERE_MAIN
        // Per-TU opt-in (chunk Q6, row 0x00E95770 CM_UndeadHere::Main): the Ego_r.pdb members (struct_layouts_egor.tsv CM_UndeadHere,
        // 32 bytes: ParentClass +0x14, PMasterData +0x18, Counter +0x1c) and the private Main override (slot 1,
        // ?Main@CM_UndeadHere@CV_HauntedHouseScript@NScript@@EAEXXZ, both PDBs). Appends members after the base: no change elsewhere.
#ifdef FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
        typedef CV_HauntedHouseScript TParentClass;
        CM_UndeadHere(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
    protected:
        // Both complete original class records: reference at14, protected data18,
        // private native signed32 counter1c. Existing pointer-view path retained.
        CV_HauntedHouseScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long Counter;
#else
        CV_HauntedHouseScript*  ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        long                    Counter;                                 // +0x1c
#endif
    private:
        virtual void Main();                                             // 0x00E95770 (slot 1)
    public:
#endif
    };
    class CHHUndead : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E95CD0
#ifdef FABLE_RETAIL_CHHUNDEAD_MEMBERS
        // Add-only opt-in (NF336, 2026-11-24): Ego_r.pdb CHHUndead (TPI field list 0x10B4C, sizeof 32): typedef TParentClass,
        // protected reference ParentClass +0x14, protected PMasterData +0x18, private float RaiseUndeadBarAmount +0x1C, public ctor
        // (+ compiler-generated copy), public virtual GetParentScript, public GetMasterData, private virtual Init / OnPredicateFail /
        // Main, compiler-generated virtual destructor (no user ~CHHUndead: ??_G 0x00E95E60 calls ~CEntityScriptBase 0x00F35B40).
        // Retail vtable 0x012ECD3C: 0 ??_G 0x00E95E60, 1 Main 0x00E95E80, 2 Init 0x00E95D80, 3 GetParentScript 0x00E95D70,
        // 4 CEntityScriptBase::OnPersist 0x00CDEBC0, 5 OnPredicateFail 0x00E95DC0, 6 CEntityScriptBase::OnInterrupted 0x00CDEBE0.
        // Spellings in both PDB name sets: ?Init@ / ?OnPredicateFail@ / ?Main@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ,
        // ?GetParentScript@CHHUndead@CV_HauntedHouseScript@NScript@@UBEPAVCScriptBase@@XZ (FableWin), ??0CHHUndead@..@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z (FableWin).
        typedef CV_HauntedHouseScript TParentClass;
        CHHUndead(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_HauntedHouseScript& ParentClass;                              // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        float RaiseUndeadBarAmount;                                      // +0x1C
        virtual void Init();                                             // 0x00E95D80 (slot 2)
        virtual void OnPredicateFail();                                  // 0x00E95DC0 (slot 5)
        virtual void Main();                                             // 0x00E95E80 (slot 1)
    public:
#endif
    };
    class CM_QuestControl : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E955B0
#ifdef FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
        // Pass26 00e96110: original nested entity layout, same base/parent/master order as CM_UndeadHere.
        // Main private virtual spelling is independently attested in Ego_r and FableWin.
        typedef CV_HauntedHouseScript TParentClass;
        CM_QuestControl(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    protected:
        CV_HauntedHouseScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
#endif
    };
    class CM_HeroInsideHouse : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E95BF0
#ifdef FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
        // Add-only opt-in (NF336, 2026-11-24): Ego_r.pdb CM_HeroInsideHouse (TPI field list 0x10B5B, sizeof 28): typedef TParentClass,
        // protected reference ParentClass +0x14, protected PMasterData +0x18, public ctor, public virtual GetParentScript, public
        // GetMasterData, private virtual Init / Main, compiler-generated virtual destructor. Retail vtable 0x012ECD20: 0 ??_G 0x00E95CB0,
        // 1 Main 0x00E96660, 2 Init 0x00E95CA0, 3 GetParentScript 0x00E95C90, 4..6 CEntityScriptBase's. FableWin.pdb spells
        // ?Init@ / ?Main@CM_HeroInsideHouse@CV_HauntedHouseScript@NScript@@EAEXXZ and ?GetParentScript@..@@UBEPAVCScriptBase@@XZ.
        typedef CV_HauntedHouseScript TParentClass;
        CM_HeroInsideHouse(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_HauntedHouseScript& ParentClass;                              // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00E95CA0 (slot 2)
        virtual void Main();                                             // 0x00E96660 (slot 1)
    public:
#endif
    };
};
}
FABLE_STATIC_ASSERT_AT(68, sizeof(NScript::CV_HauntedHouseScript) == 0x5C);

#if defined(FABLE_RETAIL_CM_UNDEADHERE_MAIN) && defined(FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS)
FABLE_STATIC_ASSERT_AT(87, sizeof(NScript::CV_HauntedHouseScript::CM_UndeadHere) == 0x20);
#endif
#ifdef FABLE_RETAIL_CHHUNDEAD_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HauntedHouseScript::CHHUndead) == 0x20);
#endif
#ifdef FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HauntedHouseScript::CM_HeroInsideHouse) == 0x1C);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_HAUNTEDHOUSESCRIPT_H
