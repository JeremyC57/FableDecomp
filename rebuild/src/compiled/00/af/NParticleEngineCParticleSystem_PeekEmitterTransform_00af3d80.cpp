// NParticleEngine::CParticleSystem::PeekEmitterTransform() const -- the parent emitter's transform, by reference
// Leaf: ?PeekEmitterTransform@CParticleSystem@NParticleEngine@@ABEABVCMatrix3x4@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3D80, 7 bytes: mov eax,[ecx+4] (PParentEmitter); add eax,0B0h; ret (thiscall, const).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?PeekEmitterTransform@CParticleSystem@NParticleEngine@@ABEABVCMatrix3x4@@XZ
//   (private const). The landed callers GetSystemTransform 0x00AF2250, GetUnscaledWorldTransform 0x00B05840 and
//   GetInverseSystemTransformWithTranslate 0x00B201D0 copy the matrix it returns. It sits right before PeekEmitterVelocity 0x00AF3DB0
//   (`add eax,52h`), its sibling accessor. Was the placeholder `long C00af3d80::M()` returning this->f + 176 (catalog DoSizeof).
// Layout: the shared engine/CParticleSystem.h (PParentEmitter +0x04) under FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM over the
//   retail engine/CParticleEmitter.h (FABLE_RETAIL_CPARTICLEEMITTER, as the landed CParticleEmitter::ReadBinary 0x00AEFAA0): Transform,
//   a CMatrix3x4 the header keeps as opaque bytes, at retail +0xb0; the same macro makes CParticleSystem a friend of the
//   emitter (Transform is protected there and neither PDB names an emitter accessor for it).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM
#include "engine/CParticleEmitter.h"
#include "engine/CParticleSystem.h"

const CMatrix3x4& NParticleEngine::CParticleSystem::PeekEmitterTransform() const
{
    return *(const CMatrix3x4*)PParentEmitter->Transform;
}
