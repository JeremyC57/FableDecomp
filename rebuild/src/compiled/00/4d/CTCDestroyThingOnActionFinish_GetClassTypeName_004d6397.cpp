// CTCDestroyThingOnActionFinish::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDestroyThingOnActionFinish::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDestroyThingOnActionFinish@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6397, 16 bytes: mov ecx,[esp+4]; call CTCDestroyThingOnActionFinish::StatGetClassTypeName 0x004D6366; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDestroyThingOnActionFinish@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123E16C (RTTI .?AVCTCDestroyThingOnActionFinish@@) slot 22 (0x0123E1C4 -> 0x004D6397) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D6366, the landed CTCDestroyThingOnActionFinish::StatGetClassTypeName (?StatGetClassTypeName@CTCDestroyThingOnActionFinish@@SI?AVCCharString@@XZ; literal "CTCDestroyThingOnActionFinish").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDestroyThingOnActionFinish_GetClassTypeName@4 calling the made-up ?Init_004d6366@T@@QAEXXZ.
// Header: no engine header defines CTCDestroyThingOnActionFinish; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCDestroyThingOnActionFinish@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCDestroyThingOnActionFinish : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D6366
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDestroyThingOnActionFinish::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
