// CTCGatherPointBuilding::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGatherPointBuilding::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGatherPointBuilding@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0B13, 16 bytes: mov ecx,[esp+4]; call CTCGatherPointBuilding::StatGetClassTypeName 0x004D4901; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGatherPointBuilding@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242264 (RTTI .?AVCTCGatherPointBuilding@@) slot 22 (0x012422BC -> 0x004E0B13) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4901, the landed CTCGatherPointBuilding::StatGetClassTypeName (?StatGetClassTypeName@CTCGatherPointBuilding@@SI?AVCCharString@@XZ; literal "CTCGatherPointBuilding").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGatherPointBuilding_GetClassTypeName@4 calling the made-up ?Init_004d4901@T@@QAEXXZ.
// Header: no engine header defines CTCGatherPointBuilding; a local class over its RTTI direct base engine/CTCGatherPointBase.h (rtti_bases.tsv: .?AVCTCGatherPointBuilding@@ : CTCGatherPointBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCGatherPointBase.h"

class CTCGatherPointBuilding : public CTCGatherPointBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4901
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGatherPointBuilding::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
