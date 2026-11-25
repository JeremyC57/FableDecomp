// CTCHaste::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCHaste::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCHaste@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0C94, 16 bytes: mov ecx,[esp+4]; call CTCHaste::StatGetClassTypeName 0x004D4AFF; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCHaste@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012423CC (RTTI .?AVCTCHaste@@) slot 22 (0x01242424 -> 0x004E0C94) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4AFF, the landed CTCHaste::StatGetClassTypeName (?StatGetClassTypeName@CTCHaste@@SI?AVCCharString@@XZ; literal "CTCHaste").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCHaste_GetClassTypeName@4 calling the made-up ?Init_004d4aff@T@@QAEXXZ.
// Header: engine/CTCHaste.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCHaste CTCHasteLayout
#include "engine/CTCHaste.h"
#undef CTCHaste

class CTCHaste : public CTCHasteLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4AFF
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCHaste::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
