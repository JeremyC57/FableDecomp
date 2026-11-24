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
#ifdef FABLE_RETAIL_COPINIONOFHERODEF_TYPENAME_PASS27
    // FableWin 01DE8660 and the original native GetSubDef at 00685C00.
    static CCharString __fastcall StatGetTypeName() { return CCharString("COpinionOfHeroDef", -1); }
#endif
    CDefIndex ThreatenGift;                                              // +0x28
    bool      WillAttackCriminals;                                       // +0x2c
    long      Personality;                                               // +0x30
    long      ReactionMask;                                              // +0x34
    long      DeedMask;                                                  // +0x38
#ifdef FABLE_RETAIL_COPINIONOFHERODEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004D3F99: ?GetSizeofClass@COpinionOfHeroDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
FABLE_STATIC_ASSERT_AT(19, offsetof(COpinionOfHeroDef, WillAttackCriminals) == 0x2c);
FABLE_STATIC_ASSERT_AT(20, sizeof(COpinionOfHeroDef) == 0x3c);
#endif
