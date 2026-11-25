// CTCGatherPointRadialArea::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGatherPointRadialArea::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGatherPointRadialArea@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0B3B, 16 bytes: mov ecx,[esp+4]; call CTCGatherPointRadialArea::StatGetClassTypeName 0x004D4914; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGatherPointRadialArea@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012422E4 (RTTI .?AVCTCGatherPointRadialArea@@) slot 22 (0x0124233C -> 0x004E0B3B) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4914, the landed CTCGatherPointRadialArea::StatGetClassTypeName (?StatGetClassTypeName@CTCGatherPointRadialArea@@SI?AVCCharString@@XZ; literal "CTCGatherPointRadialArea").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGatherPointRadialArea_GetClassTypeName@4 calling the made-up ?Init_004d4914@T@@QAEXXZ.
// Header: no engine header defines CTCGatherPointRadialArea; a local class over its RTTI direct base engine/CTCGatherPointBase.h (rtti_bases.tsv: .?AVCTCGatherPointRadialArea@@ : CTCGatherPointBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCGatherPointBase.h"

class CTCGatherPointRadialArea : public CTCGatherPointBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4914
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGatherPointRadialArea::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
