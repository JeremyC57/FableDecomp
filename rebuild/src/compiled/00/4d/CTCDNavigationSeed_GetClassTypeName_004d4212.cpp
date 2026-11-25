// CTCDNavigationSeed::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDNavigationSeed::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDNavigationSeed@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4212, 16 bytes: mov ecx,[esp+4]; call CTCDNavigationSeed::StatGetClassTypeName 0x004D41DF; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDNavigationSeed@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B944 (RTTI .?AVCTCDNavigationSeed@@) slot 22 (0x0123B99C -> 0x004D4212) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D41DF, CTCDNavigationSeed::StatGetClassTypeName (?StatGetClassTypeName@CTCDNavigationSeed@@SI?AVCCharString@@XZ; literal "CTCDNavigationSeed"; respelt by NF417 batch s1, which this row needs).
// Respell NF417 (RULINGS r1 of NF412): was the extern "C" __stdcall stand-in _CTCDNavigationSeed_GetClassTypeName@4 calling the made-up ?Init_004d41df@T@@QAEXXZ.
// Header: engine/CTCDNavigationSeed.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCDNavigationSeed CTCDNavigationSeedLayout
#include "engine/CTCDNavigationSeed.h"
#undef CTCDNavigationSeed

class CTCDNavigationSeed : public CTCDNavigationSeedLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D41DF
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDNavigationSeed::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
