#ifndef FABLE_ENGINE_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_H
#define FABLE_ENGINE_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_H
// Retail opt-in (chunk 798): NScript::CQS_GuardianTrophyDealerInfoScript, new header (no includer before chunk 798; the 13 landed
// rows keep their local models).
// Layout: Ego_r.pdb CQS_GuardianTrophyDealerInfoScript (struct_layouts_egor.tsv, 96 bytes) at retail offsets PDB - 0x14 (retail
// CLevelScriptBase is 0x48): PieceOver +0x48; 0x4c bytes. Checked against retail: CGTDI_Maze::Main 0x00E27E90 sets
// ParentClass+0x48 once the hero has met Maze.
// Nested entity script CGTDI_Maze (Ego_r.pdb, 32 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18, InitialAngle
// +0x1c): Alloc 0x00E27BB0, GetParentScript 0x00E27C50, Init 0x00E27C60 (stores the angle in +0x1c), OnPredicateFail 0x00E27C80
// (landed), Main 0x00E27E90 (?Main@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ; .rdata ref 0x012E20A8).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQS_GuardianTrophyDealerInfoScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianTrophyDealerInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQS_GuardianTrophyDealerInfoScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E27840
    virtual ~CQS_GuardianTrophyDealerInfoScript();
    virtual void RegisterMain();                                         // 0x00E27880
    virtual void Main();                                                 // 0x00E27920
    virtual void Init();                                                 // 0x00E27870
    virtual void OnPersist(CPersistContext&);                            // 0x00E27E60
    void WaitForPieceOver();                                             // 0x00E27AC0
    bool PieceOver;                                                      // +0x48
    class CGTDI_Maze;                                                    // Alloc 0x00E27BB0
};
class CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQS_GuardianTrophyDealerInfoScript&, CScriptDataBase*, const CScriptThing&); // 0x00E27BB0
    virtual CScriptBase* GetParentScript() const;                        // 0x00E27C50
private:
    virtual void Main();                                                 // 0x00E27E90
    virtual void Init();                                                 // 0x00E27C60
    virtual void OnPredicateFail();                                      // 0x00E27C80
protected:
    CQS_GuardianTrophyDealerInfoScript& ParentClass;                     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    float InitialAngle;                                                  // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_GuardianTrophyDealerInfoScript) == 0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_H
