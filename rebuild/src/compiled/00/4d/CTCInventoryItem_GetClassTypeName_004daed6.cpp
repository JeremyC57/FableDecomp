// CTCInventoryItem::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCInventoryItem::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCInventoryItem@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAED6, 16 bytes: mov ecx,[esp+4]; call CTCInventoryItem::StatGetClassTypeName 0x004D30DB; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCInventoryItem@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240004 (RTTI .?AVCTCInventoryItem@@) slot 22 (0x0124005C -> 0x004DAED6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D30DB, the landed CTCInventoryItem::StatGetClassTypeName (?StatGetClassTypeName@CTCInventoryItem@@SI?AVCCharString@@XZ; literal "CTCInventoryItem").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCInventoryItem_GetClassTypeName@4 calling the made-up ?Init_004d30db@T@@QAEXXZ.
// Header: engine/CTCInventoryItem.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCInventoryItem CTCInventoryItemLayout
#include "engine/CTCInventoryItem.h"
#undef CTCInventoryItem

class CTCInventoryItem : public CTCInventoryItemLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D30DB
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCInventoryItem::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
