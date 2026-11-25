// CTCCameraPointFixedPoint::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCameraPointFixedPoint::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCameraPointFixedPoint@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3A8F, 16 bytes: mov ecx,[esp+4]; call CTCCameraPointFixedPoint::StatGetClassTypeName 0x004D3A60; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCameraPointFixedPoint@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123AF8C (RTTI .?AVCTCCameraPointFixedPoint@@) slot 22 (0x0123AFE4 -> 0x004D3A8F) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3A60, the landed CTCCameraPointFixedPoint::StatGetClassTypeName (?StatGetClassTypeName@CTCCameraPointFixedPoint@@SI?AVCCharString@@XZ; literal "CTCCameraPointFixedPoint").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCameraPointFixedPoint_GetClassTypeName@4 calling the made-up ?Init_004d3a60@T@@QAEXXZ.
// Header: no engine header defines CTCCameraPointFixedPoint; a local class over its RTTI direct base engine/CTCCameraPointGeneralCase.h (rtti_bases.tsv: .?AVCTCCameraPointFixedPoint@@ : CTCCameraPointShapeActivationBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCCameraPointGeneralCase.h"

class CTCCameraPointFixedPoint : public CTCCameraPointShapeActivationBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3A60
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCameraPointFixedPoint::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
