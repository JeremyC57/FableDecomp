// CTCKraken::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCKraken::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCKraken@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF03D, 16 bytes: mov ecx,[esp+4]; call CTCKraken::StatGetClassTypeName 0x004D5DC2; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCKraken@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241B94 (RTTI .?AVCTCKraken@@) slot 22 (0x01241BEC -> 0x004DF03D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5DC2, the landed CTCKraken::StatGetClassTypeName (?StatGetClassTypeName@CTCKraken@@SI?AVCCharString@@XZ; literal "CTCKraken").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCKraken_GetClassTypeName@4 calling the made-up ?Init_004d5dc2@T@@QAEXXZ.
// Header: engine/CTCKraken.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCKraken CTCKrakenLayout
#include "engine/CTCKraken.h"
#undef CTCKraken

class CTCKraken : public CTCKrakenLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5DC2
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCKraken::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
