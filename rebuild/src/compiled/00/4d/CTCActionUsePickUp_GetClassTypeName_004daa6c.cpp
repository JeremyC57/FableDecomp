// CTCActionUsePickUp::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUsePickUp::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUsePickUp@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAA6C, 16 bytes: mov ecx,[esp+4]; call CTCActionUsePickUp::StatGetClassTypeName 0x004D2BA0; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUsePickUp@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F2BC (RTTI .?AVCTCActionUsePickUp@@) slot 22 (0x0123F314 -> 0x004DAA6C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2BA0, the landed CTCActionUsePickUp::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUsePickUp@@SI?AVCCharString@@XZ; literal "CTCActionUsePickUp").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUsePickUp_GetClassTypeName@4 calling the made-up ?Init_004d2ba0@T@@QAEXXZ.
// Header: no engine header defines CTCActionUsePickUp; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUsePickUp@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUsePickUp : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2BA0
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUsePickUp::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
