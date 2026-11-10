#ifndef FABLE_ENGINE_CABILITYBUTTONGUIDEF_H
#define FABLE_ENGINE_CABILITYBUTTONGUIDEF_H
// Retail model (chunk 1015, row 0x0047652B): RTTI .?AVCAbilityButtonGuiDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x0123610C (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x0046f203, 19 Copy 0x0046f49f, 20 GetSizeofClass 0x0046f1ff).
// Members: Ego_r.pdb CAbilityButtonGuiDef (96 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x5C = the element stride of TransferVectorOfSubComponents<CAbilityButtonGuiDef> 0x0047652B.
// Implicit constructor and destructor (0x004765DE CParentDefClassBase() + vtable 0x0123610C store, 0x004765FE ~CParentDefClassBase);
//   operator= is the landed implicit 0x0046F43E.
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CAbilityButtonGuiDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CAbilityButtonGuiDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CAbilityButtonGuiDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CAbilityButtonGuiDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "engine/C2DVector.h"
#ifdef _MSC_VER
enum EControllerType;                                       // Ego_r.pdb enum (MS forward declaration; enumerators not modelled)
enum EXboxControllerButton;                                       // Ego_r.pdb enum (MS forward declaration; enumerators not modelled)
enum EMouseButtonControl;                                       // Ego_r.pdb enum (MS forward declaration; enumerators not modelled)
#else
enum EControllerType { };
enum EXboxControllerButton { };
enum EMouseButtonControl { };
#endif
class CPersistContext;
class CAbilityButtonGuiDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    EControllerType ControllerType;                           // +0x3C
    EXboxControllerButton ControllerButton;                         // +0x40
    EMouseButtonControl MouseButton;                              // +0x44
    long          ButtonGraphic;                            // +0x48
    C2DVector     Position;                                 // +0x4C
    C2DVector     Offset;                                   // +0x54
};
FABLE_STATIC_ASSERT(sizeof(CAbilityButtonGuiDef) == 0x5C);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CABILITYBUTTONGUIDEF_H
