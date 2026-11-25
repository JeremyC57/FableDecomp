// CTCCarriedActionUseDrop::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseDrop::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseDrop@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DACC5, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseDrop::StatGetClassTypeName 0x004D2CB4; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseDrop@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F9A4 (RTTI .?AVCTCCarriedActionUseDrop@@) slot 22 (0x0123F9FC -> 0x004DACC5) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2CB4, the landed CTCCarriedActionUseDrop::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseDrop@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseDrop").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseDrop_GetClassTypeName@4 calling the made-up ?Init_004d2cb4@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseDrop; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseDrop@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseDrop : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2CB4
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseDrop::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
