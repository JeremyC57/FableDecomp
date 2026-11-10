#ifndef FABLE_ENGINE_CLENSFLAREELEMENTDEF_H
#define FABLE_ENGINE_CLENSFLAREELEMENTDEF_H
// Retail model (chunk 1015, row 0x004769D0): RTTI .?AVCLensFlareElementDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x01236174 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x0046f727, 19 Copy 0x0046f77c, 20 GetSizeofClass 0x0046f723).
// Members: Ego_r.pdb CLensFlareElementDef (80 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x4C = the element stride of TransferVectorOfSubComponents<CLensFlareElementDef> 0x004769D0.
// Implicit constructor and destructor (0x00476A83 CParentDefClassBase() + vtable 0x01236174 store, 0x00476AA3 ~CParentDefClassBase).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CLensFlareElementDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CLensFlareElementDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CLensFlareElementDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CLensFlareElementDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CRGBColour.h"
class CPersistContext;
class CLensFlareElementDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    float         Radius;                                   // +0x3C
    CRGBColour    Colour;                                   // +0x40
    float         Position;                                 // +0x44
    long          Texture;                                  // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CLensFlareElementDef) == 0x4C);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CLENSFLAREELEMENTDEF_H
