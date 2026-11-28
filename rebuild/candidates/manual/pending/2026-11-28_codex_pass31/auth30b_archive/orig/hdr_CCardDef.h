#ifndef FABLE_ENGINE_CCARDDEF_H
#define FABLE_ENGINE_CCARDDEF_H
// Retail model (chunk 1015, row 0x004FA0A9): RTTI .?AVCCardDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x01243A44 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x004ec0b3, 19 Copy 0x004ec0f0, 20 GetSizeofClass 0x004ec0af).
// Members: Ego_r.pdb CCardDef (72 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x44 = the element stride of TransferVectorOfSubComponents<CCardDef> 0x004FA0A9.
// Implicit constructor and destructor (0x004FA15C CParentDefClassBase() + vtable 0x01243A44 store, 0x004FA17C ~CParentDefClassBase).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CCardDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CCardDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CCardDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CCardDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CCardDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    long          CardName;                                 // +0x3C
    long          CardVal;                                  // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CCardDef) == 0x44);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CCARDDEF_H
