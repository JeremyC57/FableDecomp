// CTCCameraPointBuilding::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCameraPointBuilding::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCameraPointBuilding@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D81B6, 16 bytes: mov ecx,[esp+4]; call CTCCameraPointBuilding::StatGetClassTypeName 0x004D39EB; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCameraPointBuilding@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123ECBC (RTTI .?AVCTCCameraPointBuilding@@) slot 22 (0x0123ED14 -> 0x004D81B6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D39EB, the landed CTCCameraPointBuilding::StatGetClassTypeName (?StatGetClassTypeName@CTCCameraPointBuilding@@SI?AVCCharString@@XZ; literal "CTCCameraPointBuilding").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCameraPointBuilding_GetClassTypeName@4 calling the made-up ?Init_004d39eb@T@@QAEXXZ.
// Header: no engine header defines CTCCameraPointBuilding; a local class over its RTTI direct base engine/CTCCameraPointGeneralCase.h (rtti_bases.tsv: .?AVCTCCameraPointBuilding@@ : CTCCameraPointDefinitionBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCCameraPointGeneralCase.h"

class CTCCameraPointBuilding : public CTCCameraPointDefinitionBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D39EB
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCameraPointBuilding::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
