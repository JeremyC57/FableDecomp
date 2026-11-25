// CTCActionUseSign::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseSign::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseSign@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA9C7, 16 bytes: mov ecx,[esp+4]; call CTCActionUseSign::StatGetClassTypeName 0x004D2B7A; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseSign@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F1AC (RTTI .?AVCTCActionUseSign@@) slot 22 (0x0123F204 -> 0x004DA9C7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2B7A, the landed CTCActionUseSign::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseSign@@SI?AVCCharString@@XZ; literal "CTCActionUseSign").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseSign_GetClassTypeName@4 calling the made-up ?Init_004d2b7a@T@@QAEXXZ.
// Header: no engine header defines CTCActionUseSign; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUseSign@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUseSign : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2B7A
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseSign::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
