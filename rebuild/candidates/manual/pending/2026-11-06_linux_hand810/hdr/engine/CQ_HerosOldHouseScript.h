#ifndef FABLE_ENGINE_CQ_HEROSOLDHOUSESCRIPT_H
#define FABLE_ENGINE_CQ_HEROSOLDHOUSESCRIPT_H
// Retail opt-in (chunk 804): NScript::CQ_HerosOldHouseScript, new header (no includer before chunk 804).
// Layout: Ego_r.pdb CQ_HerosOldHouseScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): Helping +0x48, Helped +0x49, MissionAborted +0x4a,
// WifeAttacked +0x4b, WifeAttackedAndLeft +0x4c, BootyDugUp +0x4d, sizeof 0x50. Checked against retail: CGhostFisherman::Main
// 0x00D8ABF0 reads Helping / Helped / WifeAttacked and writes Helping / MissionAborted.
// Nested CGhostFisherman (Ego_r.pdb, 32 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, GhostGoing +0x1c,
// HaveTalked +0x1d. Entity vtable 0x012D50C0: 0 sdd 0x00D8A630, 1 Main 0x00D8ABF0, 2 Init 0x00D8A560, 3 GetParentScript
// 0x00D8A550, 4 OnPersist 0x00D8AB70; Alloc 0x00D8A4B0 (registration "GhostFisherman" 0x00D89F38 in Main 0x00D89E60).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // (slot 1)
    virtual void Main();                                                 // 0x00D89E60 (slot 2)
    virtual void Init();                                                 // 0x00D89DA0 (slot 3)
    bool Helping;                                                        // +0x48
    bool Helped;                                                         // +0x49
    bool MissionAborted;                                                 // +0x4a
    bool WifeAttacked;                                                   // +0x4b
    bool WifeAttackedAndLeft;                                            // +0x4c
    bool BootyDugUp;                                                     // +0x4d
    class CFishermansWife;                                               // Main 0x00D8BCD0
    class CExtraBooty;
    class CGhostFisherman : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HerosOldHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00D8A4B0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D8A550
    protected:
        CQ_HerosOldHouseScript& ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool GhostGoing;                                                 // +0x1c
        bool HaveTalked;                                                 // +0x1d
        virtual void Init();                                             // 0x00D8A560
        virtual void OnPersist(CPersistContext&);                        // 0x00D8AB70
        virtual void Main();                                             // 0x00D8ABF0
    };
};
#ifdef FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_CFISHERMANSWIFE
// Retail opt-in (chunk 810): nested CFishermansWife (Ego_r.pdb, 36 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData
// +0x18, TalkedTo +0x1c, LeavingHappy +0x1d, initial_angle +0x20. Retail: the Alloc 0x00D8A650 stores vtable 0x012D50DC (slot 0 ??_G
// 0x00D8A7D0, 1 Main 0x00D8BCD0, 2 Init 0x00D8A700, 3 GetParentScript 0x00D8A6F0, 4 OnPersist 0x00D8ABA0, 5 OnPredicateFail
// 0x00D8A770); Main 0x00D8BCD0 reads TalkedTo +0x1c / LeavingHappy +0x1d / initial_angle +0x20 and writes +0x1c / +0x1d.
// Methods: the Ego_r.pdb / FableWin.pdb name set ?<Name>@CFishermansWife@CQ_HerosOldHouseScript@NScript@@...
class CQ_HerosOldHouseScript::CFishermansWife : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HerosOldHouseScript&, CScriptDataBase*, const CScriptThing&); // 0x00D8A650
    virtual CScriptBase* GetParentScript() const;                        // 0x00D8A6F0
protected:
    CQ_HerosOldHouseScript& ParentClass;                                 // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    bool TalkedTo;                                                       // +0x1c
    bool LeavingHappy;                                                   // +0x1d
    float initial_angle;                                                 // +0x20
    virtual void Init();                                                 // 0x00D8A700
    virtual void OnPersist(CPersistContext&);                            // 0x00D8ABA0
    virtual void OnPredicateFail();                                      // 0x00D8A770
    virtual void Main();                                                 // 0x00D8BCD0
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HerosOldHouseScript) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HerosOldHouseScript::CGhostFisherman) == 0x20);
#ifdef FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_CFISHERMANSWIFE
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HerosOldHouseScript::CFishermansWife) == 0x24);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOLDHOUSESCRIPT_H
