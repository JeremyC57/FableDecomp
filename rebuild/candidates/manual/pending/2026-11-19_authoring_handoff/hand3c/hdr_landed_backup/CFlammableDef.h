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
class CFlammableDef : public CSubDefClassBase {
public:
    long  FireResistance;                                                // +0x28
    float MaxDamageCaused;                                               // +0x2c
    float FireDamagePerCycle;                                            // +0x30
    float FireDamagePeriod;                                              // +0x34
};
FABLE_STATIC_ASSERT(offsetof(CFlammableDef, MaxDamageCaused) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CFlammableDef, FireDamagePeriod) == 0x34);
#endif
