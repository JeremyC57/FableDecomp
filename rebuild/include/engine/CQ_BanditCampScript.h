#ifndef FABLE_ENGINE_CQ_BANDITCAMPSCRIPT_H
#define FABLE_ENGINE_CQ_BANDITCAMPSCRIPT_H
// Retail opt-in (chunk 806): NScript::CQ_BanditCampScript, new header (no includer before chunk 806; the landed
// CGate1GuardInner::Main 0x00D02B80 keeps its local model).
// Layout: Ego_r.pdb CQ_BanditCampScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): AttackedOuterGateGuards +0x48, Gate1Open +0x49, BoastBegun +0x4a,
// MissionSucceeded +0x4b, MissionFailed +0x4c, ActivatedBanditRaid +0x4d, sizeof 0x50 (Alloc 0x00D03580 allocates 0x50). Checked
// against retail: CGate1GuardOuter::Main 0x00D01630 reads AttackedOuterGateGuards / Gate1Open and writes both; the landed
// CGate1GuardInner::Main reads the same two offsets.
// Methods: Ego_r.pdb / FableWin.pdb ?<Name>@CQ_BanditCampScript@NScript@@ (catalog addresses).
// Nested CGate1GuardOuter (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, AIState +0x1c.
// Entity vtable 0x012C8604: 0 sdd 0x00D01570, 1 Main 0x00D01630, 2 Init 0x00D01590, 3 GetParentScript 0x00D01560; Alloc 0x00D014C0
// (allocates 0x20). CGate1GuardInner (32 bytes, same base fields): vtable 0x012C8850, Alloc 0x00D02A70, Main 0x00D02B80, Init
// 0x00D02B20, GetParentScript 0x00D02B10. Main / Init are private (PDB EAEXXZ).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00D03580
    virtual void RegisterMain();                                         // 0x00D009D0
    virtual void Main();                                                 // 0x00D00AF0
    virtual void Init();                                                 // 0x00D006D0
    virtual void OnPersist(CPersistContext&);                            // 0x00D00A90
    void CheckAnyBanditsKilled();                                        // 0x00D032F0
    void WatchForCostume();                                              // 0x00D00FD0
    void WatchForEndOfScript();
    void WatchForTermination();                                          // 0x00D01290
    bool AttackedOuterGateGuards;                                        // +0x48
    bool Gate1Open;                                                      // +0x49
    bool BoastBegun;                                                     // +0x4a
    bool MissionSucceeded;                                               // +0x4b
    bool MissionFailed;                                                  // +0x4c
    bool ActivatedBanditRaid;                                            // +0x4d
    class CGate1GuardInner;                                              // Alloc 0x00D02A70
    class CGate1GuardOuter : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampScript&, CScriptDataBase*, const CScriptThing&); // 0x00D014C0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D01560
    private:
        virtual void Main();                                             // 0x00D01630
        virtual void Init();                                             // 0x00D01590
    protected:
        CQ_BanditCampScript& ParentClass;                                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        long AIState;                                                    // +0x1c
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampScript) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampScript::CGate1GuardOuter) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_BANDITCAMPSCRIPT_H
