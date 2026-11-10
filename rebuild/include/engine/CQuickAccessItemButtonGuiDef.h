#ifndef FABLE_ENGINE_CQUICKACCESSITEMBUTTONGUIDEF_H
#define FABLE_ENGINE_CQUICKACCESSITEMBUTTONGUIDEF_H
// Retail model (chunk 1015, row 0x00476024): RTTI .?AVCQuickAccessItemButtonGuiDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x012360A4 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x0046ee6a, 19 Copy 0x0046ef87, 20 GetSizeofClass 0x0046ee66).
// Members: Ego_r.pdb CQuickAccessItemButtonGuiDef (84 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x50 = the element stride of TransferVectorOfSubComponents<CQuickAccessItemButtonGuiDef> 0x00476024.
// Implicit constructor and destructor: retail builds the default element inline (CParentDefClassBase() 0x00430370, then the
//   0x012360A4 vtable store; 0x004760D7..0x004760E8) and destroys it with ~CParentDefClassBase 0x00430300 (0x004760F7).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CQuickAccessItemButtonGuiDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CQuickAccessItemButtonGuiDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CQuickAccessItemButtonGuiDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "engine/C2DVector.h"
class CPersistContext;
class CQuickAccessItemButtonGuiDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    long          ButtonGraphic;                            // +0x3C
    C2DVector     Position;                                 // +0x40
    C2DVector     Offset;                                   // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CQuickAccessItemButtonGuiDef) == 0x50);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CQUICKACCESSITEMBUTTONGUIDEF_H
