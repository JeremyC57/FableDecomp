// CTCInventoryClothing::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCInventoryClothing::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCInventoryClothing@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7D6A, 16 bytes: mov ecx,[esp+4]; call CTCInventoryClothing::StatGetClassTypeName 0x004D2EA7; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCInventoryClothing@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124356C (RTTI .?AVCTCInventoryClothing@@) slot 22 (0x012435C4 -> 0x004E7D6A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2EA7, the landed CTCInventoryClothing::StatGetClassTypeName (?StatGetClassTypeName@CTCInventoryClothing@@SI?AVCCharString@@XZ; literal "CTCInventoryClothing").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCInventoryClothing_GetClassTypeName@4 calling the made-up ?Init_004d2ea7@T@@QAEXXZ.
// Header: engine/CTCInventoryClothing.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCInventoryClothing CTCInventoryClothingLayout
#include "engine/CTCInventoryClothing.h"
#undef CTCInventoryClothing

class CTCInventoryClothing : public CTCInventoryClothingLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2EA7
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCInventoryClothing::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
