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
        CV_HauntedHouseScript*  ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        long                    Counter;                                 // +0x1c
    private:
        virtual void Main();                                             // 0x00E95770 (slot 1)
    public:
#endif
    };
    class CHHUndead : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E95CD0
    };
    class CM_QuestControl : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E955B0
    };
    class CM_HeroInsideHouse : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HauntedHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00E95BF0
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HauntedHouseScript) == 0x5C);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_HAUNTEDHOUSESCRIPT_H
