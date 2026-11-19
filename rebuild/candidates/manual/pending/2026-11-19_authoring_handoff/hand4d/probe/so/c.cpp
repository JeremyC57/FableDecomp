// NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orient_interp) -- take a new start / end orientation: copy the interpolator, make both right-handed sets perpendicular, rebuild Transform from the start orientation (keeping its translation), InverseTransform as its rotation-only inverse, and EndTransform from the end orientation (keeping its translation)
// Grade: (in progress)
// Leaf: ?SetOrientation@CParticleEmitter@NParticleEngine@@QAEXABVC3DOrientationInterpolator@@@Z
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
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#include "engine/CParticleEmitter.h"
#include "engine/C3DOrientationInterpolator.h"

void NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orient_interp)
{
    OrientationInterpolator = orient_interp;
    OrientationInterpolator.StartOrientation.MakePerpendicular();
    OrientationInterpolator.EndOrientation.MakePerpendicular();
    Transform.Set(OrientationInterpolator.StartOrientation, Transform.GetTranslation());
    InverseTransform.SetInverseNoTranslate(Transform);
    EndTransform.Set(OrientationInterpolator.EndOrientation, EndTransform.GetTranslation());
}
