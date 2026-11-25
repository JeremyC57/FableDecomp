// NParticleEngine::CPSCDecalRenderer::GetClassTypeName() const -- the class-type name "CPSCDecalRenderer" as a CCharString
// Leaf: ?GetClassTypeName@CPSCDecalRenderer@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1FF10, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F610 "CPSCDecalRenderer"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCDecalRenderer@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x012A09BC (RTTI
//   .?AVCPSCDecalRenderer@NParticleEngine@@) slot 3 (0x012A09C8 -> 0x00B1FF10) is NParticleEngine::CPSCBase's pure `virtual CCharString
//   GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the literal
//   0x0129F610 = "CPSCDecalRenderer" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for CPSC
//   classes, so the body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCDecalRenderer_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CPSCDecalRenderer.h does not compile to the polymorphic class in any prelude P0..P3 nor with its class opt-ins
//   FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES (header debt: its default branch is not the polymorphic retail class); a local class
//   over its RTTI direct base engine/CPSCBase.h (rtti_bases.tsv: .?AVCPSCDecalRenderer@NParticleEngine@@ : NParticleEngine::CPSCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
#include "engine/CPSCBase.h"

namespace NParticleEngine {
class CPSCDecalRenderer : public CPSCBase
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCDecalRenderer::GetClassTypeName() const
{
    return CCharString("CPSCDecalRenderer", -1);
}
