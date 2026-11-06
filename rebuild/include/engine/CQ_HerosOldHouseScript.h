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
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HerosOldHouseScript) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HerosOldHouseScript::CGhostFisherman) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_HEROSOLDHOUSESCRIPT_H
