// NParticleEngine::CPSCRenderMesh::GetClassTypeName() const -- the class-type name "CPSCRenderMesh" as a CCharString
// Leaf: ?GetClassTypeName@CPSCRenderMesh@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05A50, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F524 "CPSCRenderMesh"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCRenderMesh@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x0129F96C (RTTI
//   .?AVCPSCRenderMesh@NParticleEngine@@) slot 3 (0x0129F978 -> 0x00B05A50) is NParticleEngine::CPSCBase's pure `virtual CCharString
//   GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the literal
//   0x0129F524 = "CPSCRenderMesh" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for CPSC classes,
//   so the body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCRenderMesh_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: no engine header defines NParticleEngine::CPSCRenderMesh; a local class over its RTTI direct base engine/CPSCBase.h
//   (rtti_bases.tsv: .?AVCPSCRenderMesh@NParticleEngine@@ : NParticleEngine::CPSCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#include "engine/CPSCBase.h"

namespace NParticleEngine {
class CPSCRenderMesh : public CPSCBase
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCRenderMesh::GetClassTypeName() const
{
    return CCharString("CPSCRenderMesh", -1);
}
