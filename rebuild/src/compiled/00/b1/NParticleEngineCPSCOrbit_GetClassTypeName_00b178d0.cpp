// NParticleEngine::CPSCOrbit::GetClassTypeName() const -- the class-type name "CPSCOrbit" as a CCharString
// Leaf: ?GetClassTypeName@CPSCOrbit@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B178D0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F5B8 "CPSCOrbit"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCOrbit@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x012A0374 (RTTI
//   .?AVCPSCOrbit@NParticleEngine@@) slot 3 (0x012A0380 -> 0x00B178D0) is NParticleEngine::CPSCBase's pure `virtual CCharString
//   GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the literal
//   0x0129F5B8 = "CPSCOrbit" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for CPSC classes, so the
//   body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCOrbit_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a made-up
//   `_Methods` class).
// Header: engine/NParticleEngine_CPSCOrbit.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CPSCOrbit CPSCOrbitLayout
#include "engine/NParticleEngine_CPSCOrbit.h"
#undef CPSCOrbit

namespace NParticleEngine {
class CPSCOrbit : public CPSCOrbitLayout
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCOrbit::GetClassTypeName() const
{
    return CCharString("CPSCOrbit", -1);
}
