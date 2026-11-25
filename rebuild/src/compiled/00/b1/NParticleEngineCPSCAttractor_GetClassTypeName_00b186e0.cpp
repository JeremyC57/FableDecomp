// NParticleEngine::CPSCAttractor::GetClassTypeName() const -- the class-type name "CPSCAttractor" as a CCharString
// Leaf: ?GetClassTypeName@CPSCAttractor@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B186E0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F5CC "CPSCAttractor"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCAttractor@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x012A04AC (RTTI
//   .?AVCPSCAttractor@NParticleEngine@@) slot 3 (0x012A04B8 -> 0x00B186E0) is NParticleEngine::CPSCBase's pure `virtual CCharString
//   GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the literal
//   0x0129F5CC = "CPSCAttractor" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for CPSC classes, so
//   the body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCAttractor_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: no engine header defines NParticleEngine::CPSCAttractor; a local class over its RTTI direct base engine/CPSCBase.h
//   (rtti_bases.tsv: .?AVCPSCAttractor@NParticleEngine@@ : NParticleEngine::CPSCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#include "engine/CPSCBase.h"

namespace NParticleEngine {
class CPSCAttractor : public CPSCBase
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCAttractor::GetClassTypeName() const
{
    return CCharString("CPSCAttractor", -1);
}
