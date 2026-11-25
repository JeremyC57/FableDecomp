// CTCFlourishTarget::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCFlourishTarget::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCFlourishTarget@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D96A7, 16 bytes: mov ecx,[esp+4]; call CTCFlourishTarget::StatGetClassTypeName 0x004D64A3; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCFlourishTarget@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EF34 (RTTI .?AVCTCFlourishTarget@@) slot 22 (0x0123EF8C -> 0x004D96A7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D64A3, the landed CTCFlourishTarget::StatGetClassTypeName (?StatGetClassTypeName@CTCFlourishTarget@@SI?AVCCharString@@XZ; literal "CTCFlourishTarget").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCFlourishTarget_GetClassTypeName@4 calling the made-up ?Init_004d64a3@T@@QAEXXZ.
// Header: engine/CTCFlourishTarget.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCFlourishTarget CTCFlourishTargetLayout
#include "engine/CTCFlourishTarget.h"
#undef CTCFlourishTarget

class CTCFlourishTarget : public CTCFlourishTargetLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D64A3
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCFlourishTarget::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
