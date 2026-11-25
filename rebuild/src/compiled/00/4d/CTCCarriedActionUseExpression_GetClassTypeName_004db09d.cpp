// CTCCarriedActionUseExpression::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseExpression::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseExpression@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB09D, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseExpression::StatGetClassTypeName 0x004D4B9B; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseExpression@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124026C (RTTI .?AVCTCCarriedActionUseExpression@@) slot 22 (0x012402C4 -> 0x004DB09D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4B9B, the landed CTCCarriedActionUseExpression::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseExpression@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseExpression").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseExpression_GetClassTypeName@4 calling the made-up ?Init_004d4b9b@T@@QAEXXZ.
// Header: engine/CTCCarriedActionUseExpression.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCCarriedActionUseExpression CTCCarriedActionUseExpressionLayout
#include "engine/CTCCarriedActionUseExpression.h"
#undef CTCCarriedActionUseExpression

class CTCCarriedActionUseExpression : public CTCCarriedActionUseExpressionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4B9B
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseExpression::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
