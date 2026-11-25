// CTCParticleAttacher::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCParticleAttacher::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCParticleAttacher@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE829, 16 bytes: mov ecx,[esp+4]; call CTCParticleAttacher::StatGetClassTypeName 0x004D4A43; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCParticleAttacher@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124176C (RTTI .?AVCTCParticleAttacher@@) slot 22 (0x012417C4 -> 0x004DE829) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4A43, the landed CTCParticleAttacher::StatGetClassTypeName (?StatGetClassTypeName@CTCParticleAttacher@@SI?AVCCharString@@XZ; literal "CTCParticleAttacher").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCParticleAttacher_GetClassTypeName@4 calling the made-up ?Init_004d4a43@T@@QAEXXZ.
// Header: no engine header defines CTCParticleAttacher; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCParticleAttacher@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCParticleAttacher : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4A43
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCParticleAttacher::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
