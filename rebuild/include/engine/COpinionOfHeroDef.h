#ifndef FABLE_ENGINE_COPINIONOFHERODEF_H
#define FABLE_ENGINE_COPINIONOFHERODEF_H
// COpinionOfHeroDef (acctC wfA1, row 0x00682810; new shared header): RTTI .?AVCOpinionOfHeroDef@@ single base CSubDefClassBase
// (ghidra_out/rtti_bases.tsv); Ego_r.pdb members (struct_layouts_egor.tsv, 60 bytes): ThreatenGift (CDefIndex) +0x28,
// WillAttackCriminals +0x2C, Personality +0x30, ReactionMask +0x34, DeedMask +0x38. Retail reads WillAttackCriminals at +0x2C
// (0x00682CF5); the landed models of rows 0x00958B20 / 0x007996B0 use the same offsets. Virtuals not modelled.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefIndex.h"
class COpinionOfHeroDef : public CSubDefClassBase {
public:
    CDefIndex ThreatenGift;                                              // +0x28
    bool      WillAttackCriminals;                                       // +0x2c
    long      Personality;                                               // +0x30
    long      ReactionMask;                                              // +0x34
    long      DeedMask;                                                  // +0x38
};
FABLE_STATIC_ASSERT(offsetof(COpinionOfHeroDef, WillAttackCriminals) == 0x2c);
FABLE_STATIC_ASSERT(sizeof(COpinionOfHeroDef) == 0x3c);
#endif
