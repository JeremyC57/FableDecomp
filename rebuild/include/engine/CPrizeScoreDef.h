#ifndef FABLE_ENGINE_CPRIZESCOREDEF_H
#define FABLE_ENGINE_CPRIZESCOREDEF_H
// Retail model (chunk 1015, row 0x004ED953): RTTI .?AVCPrizeScoreDef@@ : CSubDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x012437D4 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x004eac7b, 19 Copy 0x004eac9c, 20 GetSizeofClass 0x004eac55).
// Members: Ego_r.pdb CPrizeScoreDef (48 bytes), at the Ego_r offsets (the packed 0x25-byte CSubDefClassBase, first member +0x28)
//   (STLport vectors 12 bytes); sizeof 0x30 = the element stride of TransferVectorOfSubComponents<CPrizeScoreDef> 0x004ED953.
// Implicit constructor (the landed one-argument resize 0x004EDA32 calls CSubDefClassBase() 0x0044C0C0 and stores 0x012437D4).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CPrizeScoreDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CPrizeScoreDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CPrizeScoreDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CPrizeScoreDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
class CPersistContext;
class CPrizeScoreDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    float         Score;                                    // +0x28
    long          Mult;                                     // +0x2C
};
FABLE_STATIC_ASSERT(sizeof(CPrizeScoreDef) == 0x30);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CPRIZESCOREDEF_H
