// CTCDRegionEntrance::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDRegionEntrance::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDRegionEntrance@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D42D2, 16 bytes: mov ecx,[esp+4]; call CTCDRegionEntrance::StatGetClassTypeName 0x004D429B; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDRegionEntrance@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123BA74 (RTTI .?AVCTCDRegionEntrance@@) slot 22 (0x0123BACC -> 0x004D42D2) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D429B, the landed CTCDRegionEntrance::StatGetClassTypeName (?StatGetClassTypeName@CTCDRegionEntrance@@SI?AVCCharString@@XZ; literal "CTCDRegionEntrance").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDRegionEntrance_GetClassTypeName@4 calling the made-up ?Init_004d429b@T@@QAEXXZ.
// Header: no engine header defines CTCDRegionEntrance; a local class over its RTTI direct base engine/CTCDriverBase.h (rtti_bases.tsv: .?AVCTCDRegionEntrance@@ : CTCDriverBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCDriverBase.h"

class CTCDRegionEntrance : public CTCDriverBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D429B
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDRegionEntrance::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
