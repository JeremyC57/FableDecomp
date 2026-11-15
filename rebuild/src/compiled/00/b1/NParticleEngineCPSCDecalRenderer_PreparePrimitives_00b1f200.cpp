// NParticleEngine::CPSCDecalRenderer::PreparePrimitives(const float, const CEnginePrimitivePrepareInfo&, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) -- refresh the landscape / mesh decal group primitives and set or add one decal primitive per live decal (clearing dead decals)
// Inline-budget: /d2inlT1156 -- inline CDecal::SetPrimitiveInfo is expanded at retail 0x00B1FD55..0x00B1FE40; at the default budget (and at
//   T1155) VC7.1 calls it; 1156 is the smallest total budget that expands it, nothing else changes (the twin CPSCRenderSprite::PreparePrimitives
//   0x00AFCC30 carries /d2inlT1537 /d2inlS15 for the same CParticle helper).
// Grade: RELOCATION_MATCH
// Flags: /d2inlT1156
// Leaf: ?PreparePrimitives@CPSCDecalRenderer@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Retail: 0x00B1F200, 3195 bytes, thiscall, `ret 14h`; vtable 0x012A09BC slot 13 (FableWin / Ego_r name sets).
// Retail-asm: bare dword rounding `fistp dword ptr` at 0x00B1F276 (GFUnpackFloat), 0x00B1F904 (GFFloatToLongNear), 0x00B1F9A9 / 0x00B1FA1A
//   (GFFastCos), 0x00B1FB39 / 0x00B1FB54, 0x00B1FBCB (GFFloatToLongFloor) and `fld` + `fistp dword ptr [edx]` at 0x00B1FBA1..0x00B1FBA7
//   (GFFastFloatToLong); existing shared portable-twin helpers only, no new low-level helper.
// Shape: the idioms of the landed twin CPSCRenderSprite::PreparePrimitives 0x00AFCC30 (GFUnpackFloat unpacks, GFSmoothFadeValue,
//   colours[3], GetPrimID / SetPrimitiveInfo), with the decal differences read from the disassembly: the life fraction is clamped
//   to [MinLife, 1] by three assignments and written back (SetMinLife), pooled alpha / size / frame overrides (named results `alpha`,
//   `pool_size`, `size_factor`: retail evaluates the size factor first and homes both results apart from colour_fade / scale),
//   decal group primitives (type 0x1E) for the landscape and mesh handles.
// Same TU: the constructor CEnginePrimitiveParticleDecal 0x00B1FE80 (landed text) follows this function; CParticleSystem::
//   ScaleParticleDimensions(C2DVector&) is the header inline retail calls as the folded copy 0x00AF8F00. Both bodies are visible here
//   (noinline), which is what lets c2 give `colour` the dead `system_group` home (+0x7C) as retail does.
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
#define FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL
#define FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_PREPARE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#include "engine/CPSCDecalRenderer.h"
#include "engine/CDecal.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveParticleDecalGroup.h"
#include "engine/CEnginePrimitiveParticleDecal.h"
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
// Same TU (retail 0x00B1FE80 follows this function): the landed constructor's text, kept out of line as in retail.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEnginePrimitiveParticleDecal::CEnginePrimitiveParticleDecal(long decal_frame, const C3DVector& world_pos, const C2DVector& size,
                                                             const CRGBColour& colour, const CRightHandedSet& rh_set)
    : CEnginePrimitive((EEnginePrimitiveType)0x19,0,(EEngineInterpolationSet)0), WorldPos(world_pos), Size(size), Colour(colour),
      DecalFrame(decal_frame), RHSet(rh_set)
{
}
namespace NParticleEngine {
// Inline in CParticleSystem's header (FableWin.pdb), not expanded here: retail calls the folded out-of-line copy 0x00AF8F00 (as the
// landed CPSCRenderSprite::PreparePrimitives 0x00AFCC30, whose text this is); the body is visible to the TU.
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
inline float FABLE_FASTCALL GFSmoothFadeValue(float value){return 0.5f-GFFastCos(value*0.5f)*0.5f;}
void CPSCDecalRenderer::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                          const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (!DecalBankIndex) {
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
    const float tick_rate = (float)GetTickRate();       // unused: retail still calls it (0x00B1F482)
    const float one_on_tick_rate = GetOneOverTickRate();
    const CGraphicDataBank& bank = GetGraphicBank();
    const float fade_in_end = FadeInEndInteger * 0.001f;
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float fade_alpha = system.GetFadeAlpha();
    const float one_on_amin_time_secs = 1.0f / anim_time_secs;
    const float one_over_fade_in_end = 1.0f / fade_in_end;
    const float one_over_one_sub_fade_out_begin = 1.0f / (1.0f - fade_out_begin);
    const float min_alpha = FlickerMinAlphaInteger * 0.001f;
    const float min_size = FlickerMinSizeInteger * 0.001f;
    const float flicker_scale = (float)fabs(this_flicker_bias) + 0.5f;
    const float flicker_bias = this_flicker_bias + 0.5f;
    const float flicker_over_tick = one_on_tick_rate * flicker_speed;
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
    long no_frames_in_graphic = 0;
    float no_frames_in_graphic_f = 0.0f;
    float base_size_y = 0.0f;
    EEnginePrimitiveBlendOp blend_op = (EEnginePrimitiveBlendOp)BlendOp;
    EEnginePrimitiveBlendMode blend_mode = (EEnginePrimitiveBlendMode)BlendMode;
    const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
    const float one_sub_min_alpha = 1.0f - min_alpha;
    const float one_sub_min_size = 1.0f - min_size;
    if (DecalBankIndex) {
        no_frames_in_graphic = bank.GetNoFramesInGraphic(DecalBankIndex);
        no_frames_in_graphic_f = (float)no_frames_in_graphic;
        const CGraphicInfoFrame* graphic_frame = bank.PeekGraphic(DecalBankIndex)->PeekInfoFrame(0);
        base_size_y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth();
        info.Engine->SetChildPrimitive(system_group, LandscapeDecalsChildPrimIndex, LandscapeDecalsPrimHandle,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, true, blend_mode, blend_op, StencilCubeWidth, Flags), 0);
        info.Engine->SetChildPrimitive(system_group, MeshDecalsChildPrimIndex, MeshDecalsPrimHandle,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, false, blend_mode, blend_op, StencilCubeWidth, Flags), 0);
    }
    const long no_frames_in_graphic_sub_one = no_frames_in_graphic - 1;
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
    colours[0].A = (unsigned char)(colours[0].A * fade_alpha);
    colours[1].A = (unsigned char)(colours[1].A * fade_alpha);
    colours[2].A = (unsigned char)(colours[2].A * fade_alpha);
    CRGBColour colour;
    C2DVector noise_lookup;
    long child_prim_id;
    const unsigned long colour_intervals = 512;
    for (std::list<CDecal*>::iterator it_decal = system.GetDecalList().begin(); it_decal != system.GetDecalList().end(); ++it_decal) {
        CDecal& decal = **it_decal;
        const float life_timer = decal.GetInterpolatedLifetimer(interpolate);
        if (decal.GetMaxLife() > -1 && decal.IsActive()) {
            float normalised_time;
            if (life_timer / decal.GetMaxLife() < decal.GetMinLife())
                normalised_time = decal.GetMinLife();
            else if (life_timer / decal.GetMaxLife() > 1.0f)
                normalised_time = 1.0f;
            else
                normalised_time = life_timer / decal.GetMaxLife();
            decal.SetMinLife(normalised_time);
            unsigned long colour_time = GFFloatToLongNear(normalised_time * (colour_intervals - 1));
            unsigned long colour_index = colour_time >> 8;
            unsigned long colour_interp = colour_time & 0xff;
            colour = GFGetInterpolatedColour(colours[colour_index], colours[colour_index + 1], colour_interp);
            float fade_value = system.GetGlobalAlpha();
            if (normalised_time < fade_in_end)
                fade_value *= GFSmoothFadeValue(normalised_time * one_over_fade_in_end);
            if (normalised_time > fade_out_begin)
                fade_value *= GFSmoothFadeValue((1.0f - normalised_time) * one_over_one_sub_fade_out_begin);
            float colour_fade = fade_value * one_sub_alpha_fade_min + alpha_fade_min;
            float scale = fade_value * one_sub_size_fade_min + size_fade_min;
            if (FlickerEnable) {
                noise_lookup.X = life_timer * flicker_over_tick + decal.GetFlickerTimeOffset();
                noise_lookup.Y = decal.GetFlickerIndex();
                float val = perlin_noise.GetGaussian2DNoise(noise_lookup);
                val = val * flicker_scale + flicker_bias;
                if (val < 0.0f) val = 0.0f; else if (val > 1.0f) val = 1.0f;
                colour_fade *= one_sub_min_alpha * val + min_alpha;
                scale *= one_sub_min_size * val + min_size;
            }
            if (PoolingEnable && PoolIncreaseAlphaEnable) {
                const float max_alpha = MaxPoolAlpha * 255.0f;
                const float alpha = decal.GetAlphaFactor(interpolate) * colour.A * colour_fade;
                colour.A = (unsigned char)GFFloatToLongNear(std::min(alpha, max_alpha));
            } else
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * colour_fade);
            if (colour.A) {
                float anim_percent = normalised_time;
                if (ForceAnimationTime)
                    anim_percent = (float)fmod((double)((one_on_amin_time_secs * one_on_tick_rate) * life_timer), 1.0);
                long frame;
                if (PoolingEnable && PoolIncreaseFrameEnable)
                    frame = decal.GetNearestGraphicFrame();
                else
                    frame = GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f);
                frame = std::min(frame, no_frames_in_graphic_sub_one);
                C2DVector size;
                if (PoolingEnable && PoolIncreaseSizeEnable) {
                    const float size_factor = decal.GetSizeFactor(interpolate);
                    const float pool_size = size_factor * ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                    size.X = std::min(pool_size, MaxPoolSize);
                } else
                    size.X = ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                size.Y = size.X * base_size_y;
                system.ScaleParticleDimensions(size);
                if (size.X > 0.0f && size.Y > 0.0f) {
                    CEnginePrimitiveHandle& parent = decal.IsLandscapeOnlyDecal() ? LandscapeDecalsPrimHandle : MeshDecalsPrimHandle;
                    if (decal.GetPrimID(0, 0, child_prim_id))
                        info.Engine->SetChildPrimitive(parent, child_prim_id,
                            CEnginePrimitiveParticleDecal(frame, decal.GetPosition(), size, colour, decal.GetRightHandedSet()), 0);
                    else {
                        child_prim_id = info.Engine->AddChildPrimitive(parent,
                            CEnginePrimitiveParticleDecal(frame, decal.GetPosition(), size, colour, decal.GetRightHandedSet()), 0);
                        decal.SetPrimitiveInfo(0, 0, parent, child_prim_id);
                    }
                }
            }
        } else if (life_timer > decal.GetMaxLife())
            system.ClearDecal(it_decal);
    }
}
}