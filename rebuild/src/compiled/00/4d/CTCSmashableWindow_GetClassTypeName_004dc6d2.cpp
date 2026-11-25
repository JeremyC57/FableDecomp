// CTCSmashableWindow::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSmashableWindow::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSmashableWindow@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC6D2, 16 bytes: mov ecx,[esp+4]; call CTCSmashableWindow::StatGetClassTypeName 0x004D3E73; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSmashableWindow@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240BA4 (RTTI .?AVCTCSmashableWindow@@) slot 22 (0x01240BFC -> 0x004DC6D2) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3E73, the landed CTCSmashableWindow::StatGetClassTypeName (?StatGetClassTypeName@CTCSmashableWindow@@SI?AVCCharString@@XZ; literal "CTCSmashableWindow").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSmashableWindow_GetClassTypeName@4 calling the made-up ?Init_004d3e73@T@@QAEXXZ.
// Header: engine/CTCSmashableBase.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCSmashableWindow CTCSmashableWindowLayout
#include "engine/CTCSmashableBase.h"
#undef CTCSmashableWindow

class CTCSmashableWindow : public CTCSmashableWindowLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3E73
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSmashableWindow::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
