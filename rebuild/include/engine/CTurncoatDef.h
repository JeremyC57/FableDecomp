#ifndef FABLE_ENGINE_CTURNCOATDEF_RETAIL_H
#define FABLE_ENGINE_CTURNCOATDEF_RETAIL_H
// New header (egod e10, row 0x007C7C40 CTCTurncoat::FrameUpdate). Only the generated pad model engine/_quarantine/CTurncoatDef.h existed
// (guard FABLE_ENGINE_CTURNCOATDEF_H, not reused). RTTI .?AVCTurncoatDef@@ : CSubDefClassBase (landed GetSubDef<CTurncoatDef> 0x007C8000
// family); members: Ego_r.pdb names / order (struct_layouts_egor.tsv) at the retail offsets of the quarantine model (packed 0x25-byte
// base, STLport vector 12 bytes): Turncoatable +0x25 (retail 0x007C7C5D), TurncoatDeactivateEffect +0x30 (0x007C7CB1),
// TurncoatStageEffects +0x40 (0x007C7D77 / 0x007C7E03). Needs FABLE_RETAIL_DEF_CLASS_BASE and stlport/fable_containers.h.
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/CSubDefClassBase.h"
class CTurncoatDef : public CSubDefClassBase {
public:
    bool  Turncoatable;                                                  // +0x25
    long  TurncoatedEffect;                                              // +0x28
    long  TurncoatActivateEffect;                                        // +0x2c
    long  TurncoatDeactivateEffect;                                      // +0x30
    float MinimumResistance;                                             // +0x34
    float TimeDelayBeforeResistanceRecoverySecs;                         // +0x38
    float ResistanceRecoveryPerSec;                                      // +0x3c
    std::vector<long> TurncoatStageEffects;                              // +0x40
    float PercentageOfHealthAsResistance;                                // +0x4c
    float ExtraResistance;                                               // +0x50
#ifdef FABLE_RETAIL_CTURNCOATDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004DEBC0: ?GetSizeofClass@CTurncoatDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
FABLE_STATIC_ASSERT_AT(24, offsetof(CTurncoatDef, TurncoatDeactivateEffect) == 0x30);
FABLE_STATIC_ASSERT_AT(25, offsetof(CTurncoatDef, TurncoatStageEffects) == 0x40);
FABLE_STATIC_ASSERT_AT(26, sizeof(CTurncoatDef) == 0x54);
#endif // FABLE_ENGINE_CTURNCOATDEF_RETAIL_H
