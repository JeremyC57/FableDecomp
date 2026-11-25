// CTCActionUsePutInInventory::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUsePutInInventory::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUsePutInInventory@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAABC, 16 bytes: mov ecx,[esp+4]; call CTCActionUsePutInInventory::StatGetClassTypeName 0x004D2BC6; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUsePutInInventory@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F3CC (RTTI .?AVCTCActionUsePutInInventory@@) slot 22 (0x0123F424 -> 0x004DAABC) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2BC6, the landed CTCActionUsePutInInventory::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUsePutInInventory@@SI?AVCCharString@@XZ; literal "CTCActionUsePutInInventory").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUsePutInInventory_GetClassTypeName@4 calling the made-up ?Init_004d2bc6@T@@QAEXXZ.
// Header: no engine header defines CTCActionUsePutInInventory; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUsePutInInventory@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUsePutInInventory : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2BC6
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUsePutInInventory::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
