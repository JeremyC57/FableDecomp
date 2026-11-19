// NParticleEngine::CPSCSingleSprite::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, const float particleCount, const float decalCount) -- vtable slot 20: the spline base's cost when spline points are used; a sprite adds its crossed-sprite polygons and the (system-scaled) larger of the two render sizes squared; a trail adds its length and the scaled width x length
// Leaf: ?GetComponentCost@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05C60, 0x1AD bytes (padded to 432), `ret 10h`.
// Retail-asm: 0x00B05CDF + 0x00B05D39 `fistp dword ptr` -- the engine's inline-assembly GFFloatToLongNear inside the shared
//   engine/GFPackFloat.h GFUnpackFloat (its GFRoundFloatNear: the scaled size stored, reloaded, bare dword fistp); no asm in this row.
//   VC7.1 `(long)x` calls __ftol2, `/QIfist` stores `fistp qword`.
// Name: Ego_r.pdb ?GetComponentCost@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z.
// Callees: CPSCSplineBase::GetComponentCost 0x00B202C0 (Ego_r.pdb / FableWin.pdb, same signature), __ftol2 0x00BFEA70 (the float
//   StartRenderSize / EndRenderSize members handed to GFUnpackFloat's unsigned long), CParticleSystem::ScaleParticleDimensions(float&)
//   const 0x00AF8F80 (out of line, shared engine/CParticleSystem.h).
// Shape: FableWin.pdb /Od 0x02F37F40 and Ego_d.pdb's locals (start_render_size, end_render_size, render_size, poly_count, poly_cost,
//   pixel_count; trail block poly_count, trail_width, poly_cost, pixel_count): each size is ?GFUnpackFloat@@YIMKKMMK@Z(size, 11, 0, 20, 2);
//   the render cost is `poly_count + pixel_count` (retail 0x00B05DA3 `fadd [edi+0Ch] ; fadd st(1)`): the Dec 2012 donors' extra
//   `poly_count * poly_cost` (poly_cost = 1) gives `fadd st(1)` first, so this revision has no poly_cost (Ego_d's is not used);
//   the poly count is `if (CrossedSprites) ... else if (FaceMe2D) 2 else 2` -- the second test folds away but leaves the +0x93 byte
//   loaded once into al (retail 0x00B05D6E `mov al,[esi+93h] ; test al,20h`; a single test gives `test byte ptr [esi+93h],20h`).
// Layout: Ego_r.pdb / Ego_d.pdb CPSCSingleSprite (members from +0x34 over the shared CPSCSplineBase; bit fields at +0x93 / +0x94).
// Header opt-ins (hand3c overlay): FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST (the base's out-of-line override) and
//   FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE (the header-inline body VC7.1 does not inline; seeing it homes
//   render_size in the dead `system` slot like retail).
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CPSCSplineBase.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CParticleSystem.h"
#include "engine/GFPackFloat.h"

namespace NParticleEngine {

class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};

class CPSCSingleSprite : public CPSCSplineBase {
public:
    virtual void GetComponentCost(CParticleSystemCost& component_cost, const CParticleSystem& system, const float max_particle_count, const float max_decal_count);
protected:
    long SpriteBankIndex;                               // +0x34
    unsigned long StartColour;                          // +0x38 CRGBColour
    unsigned long MidColour;                            // +0x3c
    unsigned long EndColour;                            // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;                              // +0x48
    float EndRenderSize;                                // +0x4c
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;                              // +0x54
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;                           // +0x60
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;                              // +0x68
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;                                       // +0x74
    long TrailBlendMode;                                // +0x78
    long TrailBlendOp;                                  // +0x7c
    long TrailBankIndex;                                // +0x80
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;                                  // +0x91
    bool UseEndColour;                                  // +0x92
    bool SelfIlluminating : 1;                          // +0x93 bit 0
    bool AlphaFadeEnable : 1;
    bool RotateAroundCentre : 1;
    bool FaceMe2D : 1;
    bool FaceMe3D : 1;
    bool CrossedSprites : 1;
    bool SizeFadeEnable : 1;
    bool ForceAnimationTime : 1;
    bool StayWithEmitter : 1;                           // +0x94 bit 0
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
};

void CPSCSingleSprite::GetComponentCost(CParticleSystemCost& component_cost, const CParticleSystem& system, const float max_particle_count, const float max_decal_count)
{
    if (UseSplinePoints)
        CPSCSplineBase::GetComponentCost(component_cost, system, max_particle_count, max_decal_count);
    if (SpriteBankIndex) {
        float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
        float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
        float render_size = start_render_size > end_render_size ? start_render_size : end_render_size;
        system.ScaleParticleDimensions(render_size);
        float poly_count = 0.0f;
        if (CrossedSprites)
            poly_count = (float)(NoCrossedSprites * 2);
        else if (FaceMe2D)
            poly_count = 2.0f;
        else
            poly_count = 2.0f;
        float pixel_count = render_size * render_size;
        component_cost.PolyCount += poly_count;
        component_cost.PixelCount += pixel_count;
        component_cost.RenderCost += poly_count + pixel_count;
    }
    if (TrailBankIndex) {
        float poly_count = (float)((MaxTrailLength - 1) * 16);
        float trail_width = TrailWidth;
        system.ScaleParticleDimensions(trail_width);
        float pixel_count = trail_width * poly_count;
        component_cost.PolyCount += poly_count;
        component_cost.PixelCount += pixel_count;
        component_cost.RenderCost += poly_count + pixel_count;
    }
}

} // namespace NParticleEngine
