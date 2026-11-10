#ifndef FABLE_ENGINE_CCARDPOSITIONSDEF_H
#define FABLE_ENGINE_CCARDPOSITIONSDEF_H
// Retail model (chunk 1015, row 0x004F9C07): RTTI .?AVCCardPositionsDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x012439DC (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x004ebf22, 19 Copy 0x004ebebc, 20 GetSizeofClass 0x004ebe16).
// Members: Ego_r.pdb CCardPositionsDef (80 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x48 = the element stride of TransferVectorOfSubComponents<CCardPositionsDef> 0x004F9C07.
// Out-of-line constructor 0x004F9D15 and destructor 0x004F9CFC (both PDBs spell only ??1CCardPositionsDef@@UAE@XZ: virtual through
//   CParentDefClassBase); 0x004F9CBA / 0x004F9CD0 call both for the default element.
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CCardPositionsDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CCardPositionsDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CCardPositionsDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CCardPositionsDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
class CPersistContext;
class CCardPositionsDef : public CParentDefClassBase {
public:
    CCardPositionsDef();                             // 0x004F9D15 (??0CCardPositionsDef@@QAE@XZ)
    virtual ~CCardPositionsDef();                       // slot 0 0x004ebe96 (scalar deleting); ~ 0x004F9CFC (??1CCardPositionsDef@@UAE@XZ)
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    std::vector<C2DVector> Offset;                                   // +0x3C
};
FABLE_STATIC_ASSERT(sizeof(CCardPositionsDef) == 0x48);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CCARDPOSITIONSDEF_H
