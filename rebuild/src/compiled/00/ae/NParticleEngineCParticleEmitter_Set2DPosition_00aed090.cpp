// Grade: MATCH (Fable landing 2026-10-19 after the Codex review hold: test uses an EmitterModel without the real bitfields)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// CParticleEmitter::Set2DPosition(const C3DVector&, long layer) -- for a 2D emitter, set both interpolator positions (z = 0),
//   optionally the 7-bit 2D layer, and copy the positions into the start/end transforms' translations
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AED090, 167 bytes.
// Layout: PDB (Ego_r) CParticleEmitter names and bitfields, retail offset = PDB offset - 0x0c: the ushort flag word +0x40
//   (Priority:3 .. ContinuousEmitter bits 0..7, Initialised/WaitingToSleep/Sleeping/Emitter2D bits 8..11), the ushort
//   Emitter2DLayer:7 at +0x42, PositionInterpolator +0x94, Transform +0xb0, EndTransform +0x110 (CMatrix3x4 position
//   at +0x24).
// Leaf: ?Set2DPosition@CParticleEmitter@NParticleEngine@@QAEXABVC2DVector@@W4EEngine2DLayer@@@Z
// Naming (NF177, PRIORITY q11968): the local C3DVector/long CParticleEmitter model (stand-in ?Set2DPosition@CParticleEmitter@@QAEXABVC3DVector@@J@Z)
//   replaced by the shared NParticleEngine::CParticleEmitter (engine/CParticleEmitter.h retail branch, the define set of the sibling row
//   0x00AEDE60 SetOrientation); Ego_r.pdb 0x00AB1EE0 + FableWin.pdb 0x02F072A0 spelling, declared by FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D.
//   The three landed referencers (0x00543EE0, 0x00645CE9, 0x00AEB790) already bind it.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_C2DVECTOR
#include "engine/CParticleEmitter.h"
#include "engine/C2DVector.h"

void NParticleEngine::CParticleEmitter::Set2DPosition(const C2DVector& position, EEngine2DLayer layer)
{
    if (Is2DEmitter()) {
        PositionInterpolator.StartPosition.X = position.X; PositionInterpolator.StartPosition.Y = position.Y; PositionInterpolator.StartPosition.Z = 0;
        PositionInterpolator.EndPosition.X = position.X; PositionInterpolator.EndPosition.Y = position.Y; PositionInterpolator.EndPosition.Z = 0;
        if (layer != -1) Emitter2DLayer = layer;
        Transform.SetTranslateOnly(PositionInterpolator.StartPosition);
        EndTransform.SetTranslateOnly(PositionInterpolator.EndPosition);
    }
}
