// NThingShot::CTCShotDefinitionNymphFun::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionNymphFun::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionNymphFun@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCA81, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionNymphFun::StatGetClassTypeName 0x004D5401; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionNymphFun@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240E3C (RTTI .?AVCTCShotDefinitionNymphFun@NThingShot@@) slot 22 (0x01240E94 -> 0x004DCA81) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5401, the landed NThingShot::CTCShotDefinitionNymphFun::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionNymphFun@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionNymphFun").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionNymphFun_GetClassTypeName@4 calling the made-up ?Init_004d5401@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionNymphFun.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCShotDefinitionNymphFun CTCShotDefinitionNymphFunLayout
#include "engine/NThingShot_CTCShotDefinitionNymphFun.h"
#undef CTCShotDefinitionNymphFun

namespace NThingShot {
class CTCShotDefinitionNymphFun : public CTCShotDefinitionNymphFunLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5401
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionNymphFun::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
