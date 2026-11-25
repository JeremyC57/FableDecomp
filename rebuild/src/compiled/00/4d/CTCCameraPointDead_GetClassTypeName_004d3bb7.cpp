// CTCCameraPointDead::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCameraPointDead::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCameraPointDead@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3BB7, 16 bytes: mov ecx,[esp+4]; call CTCCameraPointDead::StatGetClassTypeName 0x004D3B8C; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCameraPointDead@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B23C (RTTI .?AVCTCCameraPointDead@@) slot 22 (0x0123B294 -> 0x004D3BB7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3B8C, the landed CTCCameraPointDead::StatGetClassTypeName (?StatGetClassTypeName@CTCCameraPointDead@@SI?AVCCharString@@XZ; literal "CTCCameraPointDead").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCameraPointDead_GetClassTypeName@4 calling the made-up ?Init_004d3b8c@T@@QAEXXZ.
// Header: no engine header defines CTCCameraPointDead; a local class over its RTTI direct base engine/CTCCameraPointGeneralCase.h (rtti_bases.tsv: .?AVCTCCameraPointDead@@ : CTCCameraPointShapeActivationBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCCameraPointGeneralCase.h"

class CTCCameraPointDead : public CTCCameraPointShapeActivationBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3B8C
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCameraPointDead::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
