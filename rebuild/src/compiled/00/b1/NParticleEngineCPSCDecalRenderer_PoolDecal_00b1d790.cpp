// NParticleEngine::CPSCDecalRenderer::PoolDecal(system, it_decal) -- pool a decal into the first other decal it collides with: with its normalised life clamped to [0, 1] and its pooled size (render size interpolated by that life, times its size factor, capped at MaxPoolSize), find a collider; if there is one, grow the collider's life (capped at MaxPoolLife), graphic frame, size factor (by the size ratio) and alpha factor (by the decal's colour-band alpha at its life, times the system fade alpha and its own alpha factor, capped at MaxPoolAlpha) as enabled, deactivate the decal and return true
// Leaf: ?PoolDecal@CPSCDecalRenderer@NParticleEngine@@IAE_NAAVCParticleSystem@2@AAU?$_List_iterator@PAVCDecal@NParticleEngine@@U?$_Nonconst_traits@PAVCDecal@NParticleEngine@@@std@@@std@@@Z
// Flags: none
// Retail-asm: 0x00B1D829 `fld` + 0x00B1D82D `fistp dword ptr [esp+10h]` and 0x00B1D885 + 0x00B1D889 -- the engine's inline-assembly
//   GFFloatToLongNear inside the two GFUnpackFloat expansions (engine/GFPackFloat.h). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1D790, 876 bytes, `ret 8`, esp frame.
// Shape: FableWin.pdb ?PoolDecal@CPSCDecalRenderer@NParticleEngine@@IAE_N...@Z (FableWin 0x02F56D80), statement for statement, with the
//   Ego_d.pdb locals (decal, life_timer, decal_normalised_time, decal_anim_percent, start_render_size, end_render_size, decal_size,
//   collision_decal_size, it_collison_decal, collision_decal, start_alpha, end_alpha, mid_alpha, alpha, alpha_factor, decal_alpha).
//   The debug build's range ASSERT on decal_normalised_time is not in retail. Both the life and the graphic-frame pooling test
//   PoolIncreaseLifeEnable (bit 7 of +0x34 at 0x00B1D8FF and 0x00B1D967, as FableWin 0x02F570B1 / 0x02F57145).
// Callees: DecalCollides 0x00B1D550 (same TU), NParticleEngine::GetTickRate 0x00AEA9F0 (x2), _ftol2 0x00BFEA70; the CDecal accessors,
//   GFUnpackFloat, GFGetInterpolatedValue, std::min and CParticleSystem::GetFadeAlpha are expanded.
// Same-TU helper: CPSCDecalRenderer::DecalCollides (the 0x00B1D550 row text, batch 20) precedes the row as in retail's .obj; it grades
//   RELOCATION_MATCH at its own address from this TU. Without it VC7.1 packs the frame differently (0x1C v 0x14: retail keeps
//   decal_normalised_time in the it_decal parameter slot and it_collison_decal in the system slot) and hoists the size-ratio divide
//   above IncrementSizeFactor's collision-bit test (880v876).
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_PREPARE
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CDECAL_ISACTIVE_MAXLIFE
#define FABLE_RETAIL_CDECAL_GETSIZEFACTOR_NOARG
#define FABLE_RETAIL_CDECAL_POOLING
#define FABLE_RETAIL_CDECAL_UPDATE
#define FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
#define FABLE_RETAIL_CPSCDECALRENDERER_POOLDECAL
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPSCDecalRenderer.h"
#include "engine/CDecal.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/GFPackFloat.h"
#include "engine/GFGetInterpolatedValue.h"

namespace NParticleEngine {

long FABLE_FASTCALL GetTickRate();   // 0x00AEA9F0 ?GetTickRate@NParticleEngine@@YIJXZ

// Same-TU helper: the 0x00B1D550 row (CPSCDecalRenderer::DecalCollides), verbatim.
bool CPSCDecalRenderer::DecalCollides(CParticleSystem& system, const std::list<CDecal*>::iterator& it_decal, float decal_size,
                                      std::list<CDecal*>::iterator& it_collison_decal, float& collison_decal_size)
{
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float fade_in_end = FadeInEndInteger * 0.001f;
    for (std::list<CDecal*>::iterator it_this_decal = system.GetDecalList().begin(); it_this_decal != system.GetDecalList().end(); ++it_this_decal) {
        if (it_this_decal != it_decal) {
            CDecal* this_decal = *it_this_decal;
            if (this_decal->IsActive()) {
                const float life_timer = this_decal->GetInterpolatedLifetimer(0.0f);
                const float decal_normalised_time = std::min(life_timer / this_decal->GetMaxLife(), 1.0f);
                float decal_anim_percent = decal_normalised_time;
                if (decal_normalised_time >= fade_in_end && decal_normalised_time <= fade_out_begin) {
                    CDecal* decal = *it_decal;
                    float this_decal_size = std::min(GFGetInterpolatedValue(start_render_size, end_render_size, decal_anim_percent) * this_decal->GetSizeFactor(), MaxPoolSize);
                    if ((decal->GetPosition() - this_decal->GetPosition()).GetSquaredMagnitude() < GFSqr(decal_size + this_decal_size)) {
                        collison_decal_size = this_decal_size;
                        it_collison_decal = it_this_decal;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool CPSCDecalRenderer::PoolDecal(CParticleSystem& system, std::list<CDecal*>::iterator& it_decal)
{
    CDecal* decal = *it_decal;
    const float life_timer = decal->GetInterpolatedLifetimer(0.0f);
    const float decal_normalised_time = life_timer / decal->GetMaxLife() < 0.0f ? 0.0f
                                      : (life_timer / decal->GetMaxLife() > 1.0f ? 1.0f : life_timer / decal->GetMaxLife());
    float decal_anim_percent = decal_normalised_time;
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    float decal_size = std::min(GFGetInterpolatedValue(start_render_size, end_render_size, decal_anim_percent) * decal->GetSizeFactor(), MaxPoolSize);
    float collision_decal_size = 0.0f;
    std::list<CDecal*>::iterator it_collison_decal = system.GetDecalList().end();
    if (DecalCollides(system, it_decal, decal_size, it_collison_decal, collision_decal_size)) {
        CDecal* collision_decal = *it_collison_decal;
        if (PoolIncreaseLifeEnable)
            collision_decal->SetMaxLife((long)std::min(collision_decal->GetMaxLife() + GetTickRate() * DecalLifeSecs * (1.0f - decal_normalised_time),
                                                       GetTickRate() * MaxPoolLife));
        if (PoolIncreaseLifeEnable)
            collision_decal->SetGraphicFrame(collision_decal->GetGraphicFrame() + decal->GetGraphicFrame() + PoolFrameIncreaseRate);
        if (PoolIncreaseSizeEnable)
            collision_decal->IncrementSizeFactor(decal_size / collision_decal_size);
        if (PoolIncreaseAlphaEnable) {
            const float start_alpha = UseStartColour ? StartColour.A : UseMidColour ? MidColour.A : UseEndColour ? EndColour.A : 255;
            const float end_alpha = UseEndColour ? EndColour.A : UseMidColour ? MidColour.A : UseStartColour ? StartColour.A : 255;
            const float mid_alpha = UseMidColour ? MidColour.A : (start_alpha + end_alpha) * 0.5f;
            float alpha;
            if (decal_normalised_time < 0.5f)
                alpha = (start_alpha + mid_alpha) * 0.5f;
            else
                alpha = (mid_alpha + end_alpha) * 0.5f;
            const float alpha_factor = system.GetFadeAlpha() / 255.0f;
            float decal_alpha = std::min(alpha * alpha_factor * decal->GetAlphaFactor(), MaxPoolAlpha);
            collision_decal->IncrementAlphaFactor(decal_alpha);
        }
        decal->Deactivate();
        return true;
    }
    return false;
}

}