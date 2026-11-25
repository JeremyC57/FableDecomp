// CTCCameraPointFixedDolly::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCameraPointFixedDolly::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCameraPointFixedDolly@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3ACD, 16 bytes: mov ecx,[esp+4]; call CTCCameraPointFixedDolly::StatGetClassTypeName 0x004D3AA2; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCameraPointFixedDolly@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B03C (RTTI .?AVCTCCameraPointFixedDolly@@) slot 22 (0x0123B094 -> 0x004D3ACD) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3AA2, the landed CTCCameraPointFixedDolly::StatGetClassTypeName (?StatGetClassTypeName@CTCCameraPointFixedDolly@@SI?AVCCharString@@XZ; literal "CTCCameraPointFixedDolly").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCameraPointFixedDolly_GetClassTypeName@4 calling the made-up ?Init_004d3aa2@T@@QAEXXZ.
// Header: no engine header defines CTCCameraPointFixedDolly; a local class over its RTTI direct base engine/CTCCameraPointGeneralCase.h (rtti_bases.tsv: .?AVCTCCameraPointFixedDolly@@ : CTCCameraPointShapeActivationBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCCameraPointGeneralCase.h"

class CTCCameraPointFixedDolly : public CTCCameraPointShapeActivationBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3AA2
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCameraPointFixedDolly::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
