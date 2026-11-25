// CTCDCameraPoint::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDCameraPoint::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDCameraPoint@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D8154, 16 bytes: mov ecx,[esp+4]; call CTCDCameraPoint::StatGetClassTypeName 0x004D39CF; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF406, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDCameraPoint@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EC3C (RTTI .?AVCTCDCameraPoint@@) slot 22 (0x0123EC94 -> 0x004D8154) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D39CF, the respelt CTCDCameraPoint::StatGetClassTypeName (NF402 b1: FableWin tc_register.obj + retail literal "CTCDCameraPoint").
// Respell NF406: was the extern "C" __stdcall stand-in _CTCDCameraPoint_GetClassTypeName@4 calling the made-up ?Init_004d39cf@T@@QAEXXZ.
// Header: no engine/CTCDCameraPoint.h exists; a local class over the RTTI direct base engine/CTCDriverBase.h (rtti_bases.tsv: .?AVCTCDCameraPoint@@ : CTCDriverBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCDriverBase.h"

class CTCDCameraPoint : public CTCDriverBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D39CF
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDCameraPoint::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
