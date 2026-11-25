// CTCActionUseReadable::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseReadable::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseReadable@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA9FB, 16 bytes: mov ecx,[esp+4]; call CTCActionUseReadable::StatGetClassTypeName 0x004D2B8D; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseReadable@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F234 (RTTI .?AVCTCActionUseReadable@@) slot 22 (0x0123F28C -> 0x004DA9FB) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2B8D, the landed CTCActionUseReadable::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseReadable@@SI?AVCCharString@@XZ; literal "CTCActionUseReadable").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseReadable_GetClassTypeName@4 calling the made-up ?Init_004d2b8d@T@@QAEXXZ.
// Header: engine/CTCActionUseReadable.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCACTIONUSEREADABLE_PASS24
#define FABLE_RETAIL_CREADABLEDEF_USEINFO_PASS24
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCActionUseReadable CTCActionUseReadableLayout
#include "engine/CTCActionUseReadable.h"
#undef CTCActionUseReadable

class CTCActionUseReadable : public CTCActionUseReadableLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2B8D
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseReadable::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
