// CTCResurrectionItem::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCResurrectionItem::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCResurrectionItem@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB2D7, 16 bytes: mov ecx,[esp+4]; call CTCResurrectionItem::StatGetClassTypeName 0x004D5D99; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCResurrectionItem@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240744 (RTTI .?AVCTCResurrectionItem@@) slot 22 (0x0124079C -> 0x004DB2D7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5D99, the landed CTCResurrectionItem::StatGetClassTypeName (?StatGetClassTypeName@CTCResurrectionItem@@SI?AVCCharString@@XZ; literal "CTCResurrectionItem").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCResurrectionItem_GetClassTypeName@4 calling the made-up ?Init_004d5d99@T@@QAEXXZ.
// Header: no engine header defines CTCResurrectionItem; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCResurrectionItem@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCResurrectionItem : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5D99
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCResurrectionItem::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
