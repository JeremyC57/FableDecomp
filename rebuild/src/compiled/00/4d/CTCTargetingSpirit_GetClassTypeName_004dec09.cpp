// CTCTargetingSpirit::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTargetingSpirit::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTargetingSpirit@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEC09, 16 bytes: mov ecx,[esp+4]; call CTCTargetingSpirit::StatGetClassTypeName 0x004D5853; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTargetingSpirit@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012419A4 (RTTI .?AVCTCTargetingSpirit@@) slot 22 (0x012419FC -> 0x004DEC09) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5853, the landed CTCTargetingSpirit::StatGetClassTypeName (?StatGetClassTypeName@CTCTargetingSpirit@@SI?AVCCharString@@XZ; literal "CTCTargetingSpirit").
// Respell NF412: was the stand-in ?GetClassTypeName@CTCTargetingSpirit_Calls@@QAE?AUCTargetingResult_Retail@@XZ calling the made-up ??0CTargetingResult_Retail@@QAE@XZ.
// Header: engine/CTCTargetingSpirit.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCTargetingSpirit CTCTargetingSpiritLayout
#include "engine/CTCTargetingSpirit.h"
#undef CTCTargetingSpirit

class CTCTargetingSpirit : public CTCTargetingSpiritLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5853
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTargetingSpirit::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
