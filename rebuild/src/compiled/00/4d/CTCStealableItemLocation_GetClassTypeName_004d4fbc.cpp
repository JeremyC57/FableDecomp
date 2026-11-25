// CTCStealableItemLocation::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCStealableItemLocation::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCStealableItemLocation@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4FBC, 16 bytes: mov ecx,[esp+4]; call CTCStealableItemLocation::StatGetClassTypeName 0x004D4F81; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCStealableItemLocation@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123C65C (RTTI .?AVCTCStealableItemLocation@@) slot 22 (0x0123C6B4 -> 0x004D4FBC) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4F81, the landed CTCStealableItemLocation::StatGetClassTypeName (?StatGetClassTypeName@CTCStealableItemLocation@@SI?AVCCharString@@XZ; literal "CTCStealableItemLocation").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCStealableItemLocation_GetClassTypeName@4 calling the made-up ?Init_004d4f81@T@@QAEXXZ.
// Header: no engine header defines CTCStealableItemLocation; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCStealableItemLocation@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCStealableItemLocation : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4F81
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCStealableItemLocation::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
