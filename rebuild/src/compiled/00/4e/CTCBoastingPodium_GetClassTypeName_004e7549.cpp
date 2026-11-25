// CTCBoastingPodium::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCBoastingPodium::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCBoastingPodium@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7549, 16 bytes: mov ecx,[esp+4]; call CTCBoastingPodium::StatGetClassTypeName 0x004D46D0; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCBoastingPodium@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124346C (RTTI .?AVCTCBoastingPodium@@) slot 22 (0x012434C4 -> 0x004E7549) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D46D0, the landed CTCBoastingPodium::StatGetClassTypeName (?StatGetClassTypeName@CTCBoastingPodium@@SI?AVCCharString@@XZ; literal "CTCBoastingPodium").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCBoastingPodium_GetClassTypeName@4 calling the made-up ?Init_004d46d0@T@@QAEXXZ.
// Header: engine/CTCBoastingPodium.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCBoastingPodium CTCBoastingPodiumLayout
#include "engine/CTCBoastingPodium.h"
#undef CTCBoastingPodium

class CTCBoastingPodium : public CTCBoastingPodiumLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D46D0
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCBoastingPodium::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
