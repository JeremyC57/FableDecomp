// CTCActionUseTorch::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseTorch::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseTorch@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA99F, 16 bytes: mov ecx,[esp+4]; call CTCActionUseTorch::StatGetClassTypeName 0x004D2B04; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseTorch@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F124 (RTTI .?AVCTCActionUseTorch@@) slot 22 (0x0123F17C -> 0x004DA99F) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2B04, the landed CTCActionUseTorch::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseTorch@@SI?AVCCharString@@XZ; literal "CTCActionUseTorch").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseTorch_GetClassTypeName@4 calling the made-up ?Init_004d2b04@T@@QAEXXZ.
// Header: no engine header defines CTCActionUseTorch; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUseTorch@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUseTorch : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2B04
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseTorch::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
