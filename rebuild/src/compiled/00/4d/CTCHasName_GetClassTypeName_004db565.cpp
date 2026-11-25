// CTCHasName::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCHasName::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCHasName@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB565, 16 bytes: mov ecx,[esp+4]; call CTCHasName::StatGetClassTypeName 0x004D66B0; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCHasName@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240AA4 (RTTI .?AVCTCHasName@@) slot 22 (0x01240AFC -> 0x004DB565) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D66B0, the landed CTCHasName::StatGetClassTypeName (?StatGetClassTypeName@CTCHasName@@SI?AVCCharString@@XZ; literal "CTCHasName").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCHasName_GetClassTypeName@4 calling the made-up ?Init_004d66b0@T@@QAEXXZ.
// Header: engine/CTCHasName.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCHasName CTCHasNameLayout
#include "engine/CTCHasName.h"
#undef CTCHasName

class CTCHasName : public CTCHasNameLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D66B0
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCHasName::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
