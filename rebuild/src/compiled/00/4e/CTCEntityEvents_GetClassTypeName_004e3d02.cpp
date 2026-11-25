// CTCEntityEvents::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCEntityEvents::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCEntityEvents@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3D02, 16 bytes: mov ecx,[esp+4]; call CTCEntityEvents::StatGetClassTypeName 0x004D38E0; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCEntityEvents@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242FDC (RTTI .?AVCTCEntityEvents@@) slot 22 (0x01243034 -> 0x004E3D02) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D38E0, the landed CTCEntityEvents::StatGetClassTypeName (?StatGetClassTypeName@CTCEntityEvents@@SI?AVCCharString@@XZ; literal "CTCEntityEvents").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCEntityEvents_GetClassTypeName@4 calling the made-up ?Init_004d38e0@T@@QAEXXZ.
// Header: engine/CTCEntityEvents.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCEntityEvents CTCEntityEventsLayout
#include "engine/CTCEntityEvents.h"
#undef CTCEntityEvents

class CTCEntityEvents : public CTCEntityEventsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D38E0
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCEntityEvents::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
