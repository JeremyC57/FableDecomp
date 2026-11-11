#ifndef FABLE_ENGINE_CTCCREATUREHIVE_H
#define FABLE_ENGINE_CTCCREATUREHIVE_H
// NEW (agentNF11, row 0x0077D230 CTCCreatureHive::Open): the creature-hive component. RTTI .?AVCTCCreatureHive@@ : CTCBase
// (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CTCCreatureHive (struct_layouts_egor.tsv, 0x10): Animating +0x0c (retail
// `mov byte ptr [esi+0Ch],1` at 0x0077D30A). Methods: the Ego_r.pdb / FableWin.pdb spelling the row defines (?Open@CTCCreatureHive@@QAEXXZ,
// public non-virtual). Needs FABLE_RETAIL_CTCBASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CTCCreatureHive : public CTCBase {
public:
    void Open();                                                         // 0x0077D230
private:
    bool Animating;                                                      // +0x0c
#ifdef FABLE_RETAIL_CTCCREATUREHIVE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004D45F7
public:
#endif
#ifdef FABLE_RETAIL_CTCCREATUREHIVE_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void FrameUpdate();                                          // slot 9, 0x0077D330 (tail call to UpdateAnimation)
private:
    void UpdateAnimation();                                              // 0x0077D190 ?UpdateAnimation@CTCCreatureHive@@AAEXXZ
public:
#endif // FABLE_RETAIL_CTCCREATUREHIVE_VSLOTS2
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureHive) == 0x10);
#endif
