// NParticleEngine::CPSCRenderSprite::AddToBoundingSphere -- Expand a sprite bounding sphere to include active sprites and trails.
// Grade: DIFFER(2413v2405) -- The accepted inline split removes the extra IsActive, GetRecordedPos and TransformPosIfLocal calls. The receiver allocation and frame differ from the prologue onward.
// Inline-budget: native 00AFB213, 00AFB381 and 00AFB537 call only GetSystemTransform. IsActive and both GetRecordedPos sites also expand.
// Leaf: ?AddToBoundingSphere@CPSCRenderSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Retail: 0x00AFAE50, 2405 bytes, thiscall.
// Identity: FableWin.pdb ?AddToBoundingSphere@CPSCRenderSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z;
//   slot 19 of the CPSCRenderSprite vtable (slot 19 AddToBoundingSphere of CPSCBase 0x00AF2A30 and CPSCLight 0x00B18AF0, `ret 10h`).
//   NParticleEngine::GetGraphicBank 0x00AEAAA0 (+ CGraphicDataBank slot 20 GetNoFramesInGraphic 0x00A013E0), CParticleSystem::
//   GetSystemTransform 0x00AF2250 (x3), ClearParticle 0x00AF5090, ScaleParticleDimensions(C2DVector&) 0x00AF8F00, _CIfmod.
//   GFFloatToLongCeil in GetRecordedPos -- the shared engine/GFFloatToLongNear.h / GFFastFloatToLong.h / GFFloatToLongCeil.h.
//   two-argument CMatrix3x4::TransformPoint(in, out) for the first transform only (its X/Z order differs from the tail ones).
#define FABLE_RETAIL_CPARTICLE_BOUNDING_ACTIVE_PASS24
#define FABLE_RETAIL_CPARTICLE_BOUNDING_RECORDED_PASS24
#define FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CPSCRENDERSPRITE_BOUNDING_PASS24
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/GFPackFloat.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/CPSCRenderSprite.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticle.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
namespace NParticleEngine {
long FABLE_FASTCALL GetTickRate();
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();
bool CPSCRenderSprite::AddToBoundingSphere(C3DBoundingSphere& bsphere, const float interpolate, CParticleSystem& system,
                                           const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    if (!SpriteBankIndex && !TrailBankIndex)
        return false;
    float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    float trail_width = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    float trail_length_f = TrailLengthInteger * 0.001f;
    bool trail_delay_greater_than_max = trail_length_f > 2.0f;
    float trail_stretch = (trail_delay_greater_than_max ? 2.0f : trail_length_f) * 0.5f;
    const float tick_rate=(float)GetTickRate();
    const float anim_step=1.0f/(anim_time_secs*tick_rate);
    const float normalised_time = system.GetNormalisedTime();
    const CGraphicDataBank& bank = GetGraphicBank();
    if (SpriteBankIndex)
        bank.GetNoFramesInGraphic(SpriteBankIndex);
    float anim_percent = normalised_time;
    if (anim_percent < 0.0f) anim_percent = 0.0f;
    else {
        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
        if (anim_percent > limit) anim_percent = limit;
    }
    float aspect_ratio = 0.0f;
    if (SpriteBankIndex) {
        const CGraphicInfoFrame* graphic_frame=bank.PeekGraphicInfoFrame(SpriteBankIndex,0);
        aspect_ratio=(float)graphic_frame->GetHeight()/graphic_frame->GetWidth();
    }
    bool valid = false;
    C3DVector pos;
    C3DVector trail_pos2;
    C3DVector trail_pos2_sub_pos;
    C2DVector size;
    for (CParticleSystem::ParticleIterator it_particle = system.GetParticleList().begin(); it_particle != system.GetParticleList().end(); ++it_particle) {
        const CParticle& particle = **it_particle;
        if (particle.IsActive() && particle.IsActive(interpolate)) {
            pos = particle.GetInterpolatedPos(interpolate);
            system.TransformPosIfLocal(pos);
            if (TrailBankIndex) {

                if (trail_delay_greater_than_max) {
                    trail_pos2 = particle.GetRecordedPos(2.0f, interpolate);
                    system.TransformPosIfLocal(trail_pos2);
                    trail_pos2 = pos + (trail_pos2 - pos) * trail_stretch;
                } else {
                    trail_pos2 = particle.GetRecordedPos(trail_length_f, interpolate);
                    system.TransformPosIfLocal(trail_pos2);
                }

                trail_pos2_sub_pos = trail_pos2 - pos;
                float length = trail_pos2_sub_pos.GetMagnitude();
                if (trail_width > length) length = trail_width;
                bsphere.AddToBounds(pos + trail_pos2_sub_pos * 0.5f, length * 0.5f);
                valid = true;
            }
            if (SpriteBankIndex) {
                if (ForceAnimationTime)
                    anim_percent = (float)fmod((double)(particle.GetInterpolatedLifetimer(interpolate) * anim_step), 1.0);

                size.X = (end_render_size - start_render_size) * anim_percent + start_render_size;
                size.Y = size.X * aspect_ratio;
                system.ScaleParticleDimensions(size);
                bsphere.AddToBounds(pos, (float)sqrt(size.Y * size.Y + size.X * size.X) * 0.5f);
                valid = true;
            }
        } else if (particle.GetInterpolatedLifetimer(interpolate) > particle.GetMaxLife())
            system.ClearParticle(it_particle);
    }
    if (!valid) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return valid;
}
}
