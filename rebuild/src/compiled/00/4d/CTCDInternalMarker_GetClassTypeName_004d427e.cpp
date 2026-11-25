// CTCDInternalMarker::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDInternalMarker::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDInternalMarker@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D427E, 16 bytes: mov ecx,[esp+4]; call CTCDInternalMarker::StatGetClassTypeName 0x004D4249; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDInternalMarker@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B9E4 (RTTI .?AVCTCDInternalMarker@@) slot 22 (0x0123BA3C -> 0x004D427E) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4249, the landed CTCDInternalMarker::StatGetClassTypeName (?StatGetClassTypeName@CTCDInternalMarker@@SI?AVCCharString@@XZ; literal "CTCDInternalMarker").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDInternalMarker_GetClassTypeName@4 calling the made-up ?Init_004d4249@T@@QAEXXZ.
// Header: engine/CTCDInternalMarker.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCDInternalMarker CTCDInternalMarkerLayout
#include "engine/CTCDInternalMarker.h"
#undef CTCDInternalMarker

class CTCDInternalMarker : public CTCDInternalMarkerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4249
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDInternalMarker::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
