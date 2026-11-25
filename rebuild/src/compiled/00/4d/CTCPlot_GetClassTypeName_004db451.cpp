// CTCPlot::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCPlot::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCPlot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB451, 16 bytes: mov ecx,[esp+4]; call CTCPlot::StatGetClassTypeName 0x004D608F; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCPlot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012409B4 (RTTI .?AVCTCPlot@@) slot 22 (0x01240A0C -> 0x004DB451) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D608F, the landed CTCPlot::StatGetClassTypeName (?StatGetClassTypeName@CTCPlot@@SI?AVCCharString@@XZ; literal "CTCPlot").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCPlot_GetClassTypeName@4 calling the made-up ?Init_004d608f@T@@QAEXXZ.
// Header: no engine header defines CTCPlot; a local class over its RTTI nearest ancestor with a header (direct base CTCDiceGame has none) engine/CTCBase.h (rtti_bases.tsv: .?AVCTCPlot@@ : CTCDiceGame).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCPlot : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D608F
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCPlot::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
