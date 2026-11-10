#ifndef FABLE_ENGINE_CAPPEARANCEMODIFIERSCALINGDEF_H
#define FABLE_ENGINE_CAPPEARANCEMODIFIERSCALINGDEF_H
// Retail model (chunk 1015, row 0x00479EF4): RTTI .?AVCAppearanceModifierScalingDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x01236434 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x004784e4, 19 Copy 0x0047852e, 20 GetSizeofClass 0x004784e0).
// Members: Ego_r.pdb CAppearanceModifierScalingDef (76 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x48 = the element stride of TransferVectorOfSubComponents<CAppearanceModifierScalingDef> 0x00479EF4.
// Implicit constructor and destructor (0x00479FA7 CParentDefClassBase() + vtable 0x01236434 store, 0x00479FC7 ~CParentDefClassBase).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CAppearanceModifierScalingDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CAppearanceModifierScalingDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CAppearanceModifierScalingDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CAppearanceModifierScalingDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CAppearanceModifierScalingDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    float         Attractiveness;                           // +0x3C
    float         Scariness;                                // +0x40
    float         Goodstrength;                             // +0x44
};
FABLE_STATIC_ASSERT(sizeof(CAppearanceModifierScalingDef) == 0x48);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CAPPEARANCEMODIFIERSCALINGDEF_H
