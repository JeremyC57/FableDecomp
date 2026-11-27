#ifndef FABLE_ENGINE_CFLAMMABLEDEF_H
#define FABLE_ENGINE_CFLAMMABLEDEF_H
// CFlammableDef (new header, hand3c, row 0x00780D80 CTCFlammable::FrameUpdate): RTTI .?AVCFlammableDef@@ : CSubDefClassBase
// (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv; no container before them, so the PDB offsets hold in retail:
// MaxDamageCaused +0x2C, FireDamagePerCycle +0x30, FireDamagePeriod +0x34 are read at 0x00780F09 / 0x00780EFF / 0x00780F82):
// FireResistance +0x28, then the three floats, then EffectCreationSet (CFireEffectCreationSet, opaque here). Only the members rows
// read are modelled; no size is claimed. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#ifdef FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
#include "engine/CFireEffectCreationSet.h"
#endif
class CFlammableDef : public CSubDefClassBase {
public:
#ifdef FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
    void GetAllFireHelpers(std::vector<CDefString>&) const;
    bool GetEffectAtHelperPoint(const CCharString&, long&) const;
    bool GetEffectAtHelperPoint(CDefString, long&) const;
#endif
    long  FireResistance;                                                // +0x28
    float MaxDamageCaused;                                               // +0x2c
    float FireDamagePerCycle;                                            // +0x30
#ifdef FABLE_RETAIL_FLAMMABLE_INIT_FIRE_POINTS_PASS29
    float FireDamagePeriod;
    CFireEffectCreationSet EffectCreationSet;
#else
    float FireDamagePeriod;                                              // +0x34
#endif
#ifdef FABLE_RETAIL_CFLAMMABLEDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004E2865: ?GetSizeofClass@CFlammableDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
FABLE_STATIC_ASSERT_AT(18, offsetof(CFlammableDef, MaxDamageCaused) == 0x2c);
FABLE_STATIC_ASSERT_AT(19, offsetof(CFlammableDef, FireDamagePeriod) == 0x34);
#endif
