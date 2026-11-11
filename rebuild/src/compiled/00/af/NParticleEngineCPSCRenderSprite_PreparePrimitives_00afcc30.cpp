// NParticleEngine::CPSCRenderSprite::PreparePrimitives(...) -- prepare animated particle sprites and trails
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Flags: /d2inlT1537 /d2inlS15
// Leaf: ?PreparePrimitives@CPSCRenderSprite@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Retail: 0x00AFCC30, 5274 bytes, thiscall.
// Identity: original PDB signatures; native vtable0129F67C slot13.
// Retail-asm: bare dword rounding `fistp dword ptr` at 0x00AFCCAC, 0x00AFD424, 0x00AFD846, 0x00AFDCE5 (GFFloatToLongNear / GFFloatToLongFloor);
// use existing shared portable-twin helpers only. No new low-level helper.
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#include "engine/CPSCRenderSprite.h"
#include "engine/CParticle.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveParticleSpriteGroup.h"
#include "engine/CEnginePrimitiveParticleSpriteTrailGroup.h"
#include "engine/CEnginePrimitiveParticleSprite.h"
#include "engine/CEnginePrimitiveParticleLine.h"
#include "engine/GFPackFloat.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include <math.h>
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index)const{return index<GraphicList.size()?&GraphicList[index]:0;}
inline unsigned long CGraphicInfoFrame::GetWidth()const{return GraphicInfo->Header.FrameWidth;}
inline unsigned long CGraphicInfoFrame::GetHeight()const{return GraphicInfo->Header.FrameHeight;}
#include "engine/GFFastCos.h"
CRGBColour GFGetInterpolatedColour(CRGBColour,CRGBColour,unsigned long);
// Original donor02F2A890 reads all input fields and initializes the complete sprite.
// Native AFD992/AFD9DD/AFDB75/AFDBBA call this real constructor.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEnginePrimitiveParticleSprite::CEnginePrimitiveParticleSprite(unsigned __int64 uid,unsigned long frame,
    const C3DVector& world_pos,const C2DVector& size,float angle,const CRGBColour& colour,EEngineInterpolationSet interp_set)
    :CEnginePrimitive((EEnginePrimitiveType)0x16,uid,interp_set),Frame(frame),WorldPos(world_pos),Size(size),Angle(angle),Colour(colour){}
// Original donor02F2A810; native AFDF34/AFDF75 calls00AFE220.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEnginePrimitiveParticleLine::CEnginePrimitiveParticleLine(unsigned __int64 uid,const C3DVector& start,
    const C3DVector& end,const CRGBColour& colour,float width,EEngineInterpolationSet interp_set)
    :CEnginePrimitive((EEnginePrimitiveType)0x15,uid,interp_set),Start(start),End(end),Colour(colour),Width(width){}
namespace NParticleEngine {
// Original donor02F26DC0/02F27D10; native AF8F80/AF8F00 are called.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(float& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim*=scale;}
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(C2DVector& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim.X*=scale;dim.Y*=scale;}
}
long FABLE_FASTCALL GetTickRate();
float FABLE_FASTCALL GetOneOverTickRate();
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
// Original donor02F2ACA0 delegates to GFFastCos; value is PDB local.
inline float FABLE_FASTCALL GFSmoothFadeValue(float value){return 0.5f-GFFastCos(value*0.5f)*0.5f;}
void CPSCRenderSprite::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (!SpriteBankIndex && !TrailBankIndex) {
        ClearAllPrimitives(info.Engine, system_group);
        return;
    }
    const float alpha_fade = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float size_fade = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float flicker_speed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
    const float this_flicker_bias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
    const float trail_width = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    const float tick_rate=(float)GetTickRate();
    const float one_on_tick_rate = GetOneOverTickRate();
    const CGraphicDataBank& bank = GetGraphicBank();
    const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
    const float trail_length_f = TrailLengthInteger * 0.001f;
    const float trail_max_delay=2.0f;
    const float one_on_trail_max_delay=1.0f/trail_max_delay;
    const bool trail_delay_greater_than_max=trail_length_f>trail_max_delay;
    const float trail_delay=trail_delay_greater_than_max?trail_max_delay:trail_length_f;
    const float trail_stretch=trail_delay*one_on_trail_max_delay;
    const float fade_in_end = FadeInEndInteger * 0.001f;
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float min_alpha = FlickerMinAlphaInteger * 0.001f;
    const float min_size = FlickerMinSizeInteger * 0.001f;
    const float flicker_scale = (float)fabs(this_flicker_bias) + 0.5f;
    const float flicker_bias = this_flicker_bias + 0.5f;
    const float flicker_over_tick = one_on_tick_rate * flicker_speed;
    const float one_on_amin_time_secs = 1.0f / anim_time_secs;
    const float one_on_num_crossed_sprites = 1.0f / NoCrossedSprites;
    const float one_over_fade_in_end = 1.0f / fade_in_end;
    const float one_over_one_sub_fade_out_begin = 1.0f / (1.0f - fade_out_begin);
    const float one_sub_min_alpha = 1.0f - min_alpha;
    const float one_sub_min_size = 1.0f - min_size;
    const float fade_alpha = system.GetFadeAlpha();
    float alpha_fade_min, one_sub_alpha_fade_min;
    if (AlphaFadeEnable) {
        one_sub_alpha_fade_min = 1.0f - alpha_fade;
        alpha_fade_min = alpha_fade;
    } else {
        one_sub_alpha_fade_min = 0.0f;
        alpha_fade_min = 1.0f;
    }
    float size_fade_min, one_sub_size_fade_min;
    if (SizeFadeEnable) {
        one_sub_size_fade_min = 1.0f - size_fade;
        size_fade_min = size_fade;
    } else {
        one_sub_size_fade_min = 0.0f;
        size_fade_min = 1.0f;
    }
    const float crossed_sprite_full_angle=((Flags&2)||(Flags&4))?1.0f:0.5f;
    long particle_prim_handle_index=0;
    long particle_trail_prim_handle_index = 0;
    long no_frames_in_graphic = 0;
    float no_frames_in_graphic_f = 0.0f;
    const CGraphicInfoFrame* graphic_frame=0;
    float base_size_y = 0.0f;
    EEnginePrimitiveBlendOp blend_op = (EEnginePrimitiveBlendOp)BlendOp;
    EEnginePrimitiveBlendMode blend_mode = (EEnginePrimitiveBlendMode)BlendMode;
    EEnginePrimitiveBlendOp trail_blend_op = (EEnginePrimitiveBlendOp)TrailBlendOp;
    EEnginePrimitiveBlendMode trail_blend_mode = (EEnginePrimitiveBlendMode)TrailBlendMode;
    if (SpriteBankIndex) {
        no_frames_in_graphic = bank.GetNoFramesInGraphic(SpriteBankIndex);
        no_frames_in_graphic_f = (float)no_frames_in_graphic;
        graphic_frame = bank.PeekGraphic(SpriteBankIndex)->PeekInfoFrame(0);
        base_size_y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth();
        info.Engine->SetChildPrimitive(system_group, ChildPrimIndex, PrimHandle,
            CEnginePrimitiveParticleSpriteGroup(SpriteBankIndex, Flags, (EEnginePrimitiveBlendMode)blend_mode,
                                                (EEnginePrimitiveBlendOp)blend_op), 0);
        particle_trail_prim_handle_index = 1;
    }
    const long no_frames_in_graphic_sub_one = no_frames_in_graphic - 1;
    if (TrailBankIndex)
        info.Engine->SetChildPrimitive(system_group, ChildTrailPrimIndex, TrailPrimHandle,
            CEnginePrimitiveParticleSpriteTrailGroup(TrailBankIndex, Flags, (EEnginePrimitiveBlendMode)trail_blend_mode,
                                                     (EEnginePrimitiveBlendOp)trail_blend_op), 0);
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
    colours[0].A = (unsigned char)(colours[0].A * fade_alpha);
    colours[1].A = (unsigned char)(colours[1].A * fade_alpha);
    colours[2].A = (unsigned char)(colours[2].A * fade_alpha);
    CRGBColour colour;
    C3DVector pos,trail_pos_2;
    C2DVector noise_lookup;

    long child_prim_id;
    const unsigned long colour_intervals=512;
    for (CParticleSystem::ParticleIterator it_particle = system.GetParticleList().begin(); it_particle != system.GetParticleList().end(); ++it_particle) {
        CParticle& particle = **it_particle;
        const float life_timer = particle.GetInterpolatedLifetimer(interpolate);
        if (particle.IsActive() && particle.IsActive(interpolate) && particle.GetMaxLife()) {
            const float normalised_time = life_timer/particle.GetMaxLife()<0.0f?0.0f:life_timer/particle.GetMaxLife()>1.0f?1.0f:life_timer/particle.GetMaxLife();
            unsigned long colour_time = GFFloatToLongNear(normalised_time * (colour_intervals-1));
            unsigned long colour_index = colour_time >> 8;
            unsigned long colour_interp=colour_time&0xff;
            colour = GFGetInterpolatedColour(colours[colour_index], colours[colour_index + 1], colour_interp);
            float fade_value = system.GetGlobalAlpha();
            if (normalised_time < fade_in_end)
                fade_value *= GFSmoothFadeValue(normalised_time * one_over_fade_in_end);
            if (normalised_time > fade_out_begin)
                fade_value *= GFSmoothFadeValue((1.0f - normalised_time) * one_over_one_sub_fade_out_begin);
            float colour_fade = fade_value * one_sub_alpha_fade_min + alpha_fade_min;
            float scale = fade_value * one_sub_size_fade_min + size_fade_min;
            if (FlickerEnable) {
                noise_lookup.X = life_timer * flicker_over_tick + particle.GetFlickerTimeOffset();
                noise_lookup.Y = particle.GetFlickerIndex();
                float val=perlin_noise.GetGaussian2DNoise(noise_lookup);
                val=val*flicker_scale+flicker_bias;
                if(val<0.0f)val=0.0f;else if(val>1.0f)val=1.0f;
                colour_fade *= one_sub_min_alpha * val + min_alpha;
                scale *= one_sub_min_size * val + min_size;
            }
            colour.A = (unsigned char)GFFloatToLongNear(colour.A * colour_fade);
            if (colour.A) {
                pos = particle.GetInterpolatedPos(interpolate);
                system.TransformPosIfLocal(pos);
                if (SpriteBankIndex) {
                    float anim_percent = normalised_time;
                    // NativeAFD7ED multiplies the two reciprocal rates before the lifetime.
                    if (ForceAnimationTime)
                        anim_percent = (float)fmod((double)((one_on_amin_time_secs * one_on_tick_rate) * life_timer), 1.0);
                    const float sprite_angle = particle.GetAngleChange() * interpolate + particle.GetAngle();
                    const long frame = GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f) < no_frames_in_graphic_sub_one ? GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f) : no_frames_in_graphic_sub_one;
                    C2DVector size;
                    size.X = ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                    size.Y = size.X * base_size_y;
                    system.ScaleParticleDimensions(size);
                    if (Flags & 0x40) {
                        for (long c_sprite = 0; c_sprite < NoCrossedSprites; ++c_sprite) {
                            const float angle = (c_sprite + 0.5f) * crossed_sprite_full_angle * one_on_num_crossed_sprites + sprite_angle;

                            if (particle.GetPrimID(particle_prim_handle_index, c_sprite, child_prim_id))
                                info.Engine->SetChildPrimitive(PrimHandle, child_prim_id,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0);
                            else {
                                // NativeAFDA2C uses the existing result slot for both lookup and creation.
                                child_prim_id = info.Engine->AddChildPrimitive(PrimHandle,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0);
                                particle.SetPrimitiveInfo(particle_prim_handle_index, c_sprite, PrimHandle, child_prim_id);
                            }
                        }
                    } else {

                        if (particle.GetPrimID(particle_prim_handle_index, 0, child_prim_id))
                            info.Engine->SetChildPrimitive(PrimHandle, child_prim_id,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, sprite_angle, colour, (EEngineInterpolationSet)0), 0);
                        else {
                            child_prim_id = info.Engine->AddChildPrimitive(PrimHandle,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, sprite_angle, colour, (EEngineInterpolationSet)0), 0);
                            particle.SetPrimitiveInfo(particle_prim_handle_index, 0, PrimHandle, child_prim_id);
                        }
                    }
                }
                if (TrailBankIndex) {
                    trail_pos_2 = particle.GetRecordedPos(trail_delay, interpolate);
                    if (trail_delay_greater_than_max)
                        trail_pos_2 = pos + (trail_pos_2 - pos) * trail_stretch;
                    system.TransformPosIfLocal(trail_pos_2);
                    float width = scale * trail_width;
                    system.ScaleParticleDimensions(width);

                    if (particle.GetPrimID(particle_trail_prim_handle_index, 0, child_prim_id))
                        info.Engine->SetChildPrimitive(TrailPrimHandle, child_prim_id,
                            CEnginePrimitiveParticleLine(0, pos, trail_pos_2, colour, width, (EEngineInterpolationSet)0), 0);
                    else {
                        child_prim_id = info.Engine->AddChildPrimitive(TrailPrimHandle,
                            CEnginePrimitiveParticleLine(0, pos, trail_pos_2, colour, width, (EEngineInterpolationSet)0), 0);
                        particle.SetPrimitiveInfo(particle_trail_prim_handle_index, 0, TrailPrimHandle, child_prim_id);
                    }
                }
            }
        } else if (life_timer > particle.GetMaxLife())
            system.ClearParticle(it_particle);
    }
}
}