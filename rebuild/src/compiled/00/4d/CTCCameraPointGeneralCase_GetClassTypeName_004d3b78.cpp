// CTCCameraPointGeneralCase::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCameraPointGeneralCase::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCameraPointGeneralCase@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3B78, 16 bytes: mov ecx,[esp+4]; call CTCCameraPointGeneralCase::StatGetClassTypeName 0x004D3B2E; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCameraPointGeneralCase@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B194 (RTTI .?AVCTCCameraPointGeneralCase@@) slot 22 (0x0123B1EC -> 0x004D3B78) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3B2E, the landed CTCCameraPointGeneralCase::StatGetClassTypeName (?StatGetClassTypeName@CTCCameraPointGeneralCase@@SI?AVCCharString@@XZ; literal "CTCCameraPointGeneralCase").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCameraPointGeneralCase_GetClassTypeName@4 calling the made-up ?Init_004d3b2e@T@@QAEXXZ.
// Header: engine/CTCCameraPointGeneralCase.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCCameraPointGeneralCase CTCCameraPointGeneralCaseLayout
#include "engine/CTCCameraPointGeneralCase.h"
#undef CTCCameraPointGeneralCase

class CTCCameraPointGeneralCase : public CTCCameraPointGeneralCaseLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3B2E
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCameraPointGeneralCase::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
