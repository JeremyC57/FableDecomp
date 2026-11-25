// NParticleEngine::CPSCEmitterGeneric::GetClassTypeName() const -- the class-type name "CPSCEmitterGeneric" as a CCharString
// Leaf: ?GetClassTypeName@CPSCEmitterGeneric@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15A90, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F594 "CPSCEmitterGeneric"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCEmitterGeneric@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x012A002C (RTTI
//   .?AVCPSCEmitterGeneric@NParticleEngine@@) slot 3 (0x012A0038 -> 0x00B15A90) is NParticleEngine::CPSCBase's pure `virtual
//   CCharString GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the
//   literal 0x0129F594 = "CPSCEmitterGeneric" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for
//   CPSC classes, so the body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCEmitterGeneric_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CPSCEmitterGeneric.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define CPSCEmitterGeneric CPSCEmitterGenericLayout
#include "engine/CPSCEmitterGeneric.h"
#undef CPSCEmitterGeneric

namespace NParticleEngine {
class CPSCEmitterGeneric : public CPSCEmitterGenericLayout
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCEmitterGeneric::GetClassTypeName() const
{
    return CCharString("CPSCEmitterGeneric", -1);
}
