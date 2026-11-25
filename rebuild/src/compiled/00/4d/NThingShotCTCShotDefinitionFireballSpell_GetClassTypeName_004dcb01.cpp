// NThingShot::CTCShotDefinitionFireballSpell::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionFireballSpell@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCB01, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName 0x004D553C; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionFireballSpell@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241064 (RTTI .?AVCTCShotDefinitionFireballSpell@NThingShot@@) slot 22 (0x012410BC -> 0x004DCB01) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D553C, the landed NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionFireballSpell@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionFireballSpell").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionFireballSpell_GetClassTypeName@4 calling the made-up ?Init_004d553c@T@@QAEXXZ.
// Header: no engine header defines NThingShot::CTCShotDefinitionFireballSpell; a local class over its RTTI direct base engine/NThingShot_CTCShotDefinitionProjectilePredictive.h (rtti_bases.tsv: .?AVCTCShotDefinitionFireballSpell@NThingShot@@ : NThingShot::CTCShotDefinitionProjectile).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"

namespace NThingShot {
class CTCShotDefinitionFireballSpell : public CTCShotDefinitionProjectile
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D553C
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionFireballSpell::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
