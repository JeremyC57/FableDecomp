// CEngineInternalPrimitiveRepeatedZSprite::GetTransitionFade() const -- the z-sprite's fade-in range with its end clamped to the renderer's z-sprite-as-mesh distance window and its start moved by the same amount
// Leaf: GetTransitionFade
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAA460, 112 bytes, thiscall, `ret 4` (CEngineFadeDistance returned through the
//   hidden pointer).
// Identity: Ego_r.pdb ?GetTransitionFade@CEngineInternalPrimitiveRepeatedZSprite@@QBE?AVCEngineFadeDistance@@XZ; callers
//   are the landed IsInsideFrustum 0x00BAA4E0 (call 0x00BAA53E, which reads the result's start) and 0x00BAB13B. Catalog
//   "engine_get_clamped_z_coordinate" is descriptive.
// Layout: engine/CEngineInternalPrimitiveRepeatedZSprite.h FadeInStart +0x54, FadeInEnd +0x58; the global 0x01436E80 is
//   the renderer's shared data (Ego_r.pdb CEnginePrimitiveRendererSharedData: ZSpriteDrawDistanceFactor +0x8C -- the
//   landed 0x00BAA4E0's scale --, MinimumZSpriteAsMeshDistance +0x90, MaximumZSpriteAsMeshDistance +0x94).
//   CEngineFadeDistance (Ego_r.pdb) {FadeBeginDistance, MaxDrawDistance} needs a user constructor so it is returned
//   through the hidden pointer.
// Shape: a local CEngineFadeDistance built from the two members (begin kept on the x87 stack, end in the frame), the end
//   clamped by an inline by-value Clamp (`value < lo ? lo : (value > hi ? hi : value)`; the high bound copied to the
//   frame), the begin moved by (old end - clamped end), the local copied out (no NRVO).
#include "engine/CEngineInternalPrimitiveRepeatedZSprite.h"

class CEngineFadeDistance {                        // Ego_r.pdb, 8 bytes
public:
    float FadeBeginDistance;                       // +0x00
    float MaxDrawDistance;                         // +0x04
    CEngineFadeDistance(float fadeBegin, float maxDraw) : FadeBeginDistance(fadeBegin), MaxDrawDistance(maxDraw) { }
};

struct CEnginePrimitiveRendererSharedData {        // Ego_r.pdb (only the z-sprite distances)
    unsigned char _pad00[0x8c];
    float ZSpriteDrawDistanceFactor;               // +0x8c
    float MinimumZSpriteAsMeshDistance;            // +0x90
    float MaximumZSpriteAsMeshDistance;            // +0x94
};
extern CEnginePrimitiveRendererSharedData* g_pEnginePrimitiveRendererSharedData;   // 0x01436E80

template <class T> inline T Clamp(T value, T lo, T hi)
{
    return value < lo ? lo : (value > hi ? hi : value);
}

struct CEngineInternalPrimitiveRepeatedZSprite_Methods : CEngineInternalPrimitiveRepeatedZSprite {
    CEngineFadeDistance GetTransitionFade() const;
};

CEngineFadeDistance CEngineInternalPrimitiveRepeatedZSprite_Methods::GetTransitionFade() const
{
    CEngineFadeDistance fade(FadeInStart, FadeInEnd);
    float transitionEnd = Clamp(fade.MaxDrawDistance, g_pEnginePrimitiveRendererSharedData->MinimumZSpriteAsMeshDistance,
                                g_pEnginePrimitiveRendererSharedData->MaximumZSpriteAsMeshDistance);
    fade.FadeBeginDistance -= fade.MaxDrawDistance - transitionEnd;
    fade.MaxDrawDistance = transitionEnd;
    return fade;
}