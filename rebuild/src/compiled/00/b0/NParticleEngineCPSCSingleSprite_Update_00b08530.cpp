// NParticleEngine::CPSCSingleSprite::Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool&) -- advances sprite trails and keeps active timelines alive.
// Leaf: ?Update@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z
// Flags: default
// Inline-budget: The two retail world-space adjustments expand at the caller. The end wrapper stays out of line without its scoped opt-in.
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_C2DVECTOR_INLINE
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM_NESTED
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_ADDTOBOUNDINGSPHERE
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPARTICLESYSTEM_TRANSFORMPOSIFWORLDEND
#define FABLE_RETAIL_CPARTICLESYSTEM_TRANSFORMPOSIFWORLDEND_INLINE
#include "engine/CParticleSystem.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CPSCSingleSprite.h"

namespace NParticleEngine {
void CPSCSingleSprite::Update(CParticleSystem& system,
    const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& keep_alive)
{
    if (!EnteringTimelineEnd || GetApproximateSplineLength() > 0.0f)
        keep_alive = true;

    C3DVector pos;
    if (Lifetimer == 0 && UsePosition) {
        TrailUpdatePrimitive.Trail.clear();
        if (params.GetUpdateParam(pos, PositionParam, 0, false, 0)) {
            system.TransformPosIfWorld(pos);
            AddTrailPosition(pos, TrailWidth);
        }
    }
    if (UsePosition) {
        if (params.GetUpdateParam(pos, PositionParam, 1, false, 0)) {
            system.TransformPosIfWorldEnd(pos);
            AddTrailPosition(pos, TrailWidth);
        }
    } else if ((SplineRandomOffsetData.IsNull() && !SplinePosParamData.IsNull())
               || TrailUpdatePrimitive.Trail.size() == 0) {
        SetTrailPositionsFromSpline(system, params, TrailWidth, 0);
    }
    ++Lifetimer;
}
}