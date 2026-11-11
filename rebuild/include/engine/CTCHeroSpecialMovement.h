#ifndef FABLE_ENGINE_CTCHEROSPECIALMOVEMENT_H
#define FABLE_ENGINE_CTCHEROSPECIALMOVEMENT_H
// NEW (agentNF10, row 0x007E0910 CTCHeroSpecialMovement::OnCreate): the hero's sprint / run component. RTTI .?AVCTCHeroSpecialMovement@@ :
// CTCBase (ghidra_out/rtti_bases.tsv), vtable 0x0126FE3C (slot 4 OnCreate 0x007E0910). Members: Ego_r.pdb CTCHeroSpecialMovement
// (struct_layouts_egor.tsv, 0x1C, no STL member: the same offsets in retail; OnCreate stores Sprinting `[esi+0Ch]`, SprintFramesRemaining
// `[esi+10h]` and reads PHeroSpecialMovementDef `[esi+18h]`). Methods: the Ego_r.pdb / FableWin.pdb spellings the rows define.
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CHeroSpecialMovementDef;
class CTCHeroSpecialMovement : public CTCBase {
public:
    virtual void OnCreate();                                             // slot 4 0x007E0910 ?OnCreate@CTCHeroSpecialMovement@@UAEXXZ
private:
    bool  Sprinting;                                                     // +0x0c
    bool  Running;                                                       // +0x0d
    float SprintFramesRemaining;                                         // +0x10
    long  RunTimer;                                                      // +0x14
    CDefPointer<const CHeroSpecialMovementDef> PHeroSpecialMovementDef;  // +0x18
#ifdef FABLE_RETAIL_CTCHEROSPECIALMOVEMENT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007E0A30
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroSpecialMovement) == 0x1c);
#endif
