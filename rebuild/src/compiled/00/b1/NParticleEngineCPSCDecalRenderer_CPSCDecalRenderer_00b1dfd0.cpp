// NParticleEngine::CPSCDecalRenderer::CPSCDecalRenderer() -- default decal renderer component: a 1 s decal life, no decal bank, white colours, the database defaults packed into the bit-field parameters (render size 1, animation 1 s, flicker speed 15, ...), pooling on (size / life to 5, alpha / frame rate 1), a 0.2 stencil cube, two empty primitive handles, and the emitter manager's default alignment / blend mode / blend op
// Leaf: ??0CPSCDecalRenderer@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B1E071 `fld dword ptr [esp+10h]` + 0x00B1E075 `fistp dword ptr [esp+0Ch]` (14 sites) -- the engine's inline-assembly
//   GFFloatToLongNear inside the shared engine/GFPackFloat.h quantiser (round to two decimals, then pack); no asm in this row.
//   VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1DFD0, 931 bytes, plain `ret`.
// Name: Ego_r.pdb 0x00ADE5E0 + FableWin.pdb 0x02F55400 ??0CPSCDecalRenderer@NParticleEngine@@QAE@XZ; vtable 0x012A09BC.
// Callees: CPSCBase::CPSCBase() 0x00AF2620, CBaseClass() 0x0099A2D0 (the two CEnginePrimitiveHandle members, vtable 0x0122F598),
//   NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0 -- called three times for its inline const GetDefaultAlignmentMode /
//   GetDefaultBlendMode / GetDefaultBlendOp (FableWin.pdb), whose results fold to constants.
// Layout: the shared engine/CPSCDecalRenderer.h (Ego_r.pdb members, retail offsets).
// Shape: FableWin.pdb's /Od body 0x02F55400: every member in the mem-initialiser list in declaration order (each packed float is
//   GFPackFloat(value, bits, min, max, 2): ecx = bits, edx = 2), then Flags / BlendMode / BlendOp from the emitter manager in the body.
//   The shared engine/GFPackFloat.h with FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT, as the landed CPSCDecalRenderer rows
//   (DatabaseSerialise 0x00B1E380, PreparePrimitives 0x00B1F200).
// Header opt-ins (hand3c overlay): FABLE_RETAIL_CPSCDECALRENDERER_CTOR, FABLE_RETAIL_CPSCBASE_CTOR,
//   FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTALIGNMENT.
// Constants: 1.0f 0x0122DED8 (and 0x3F800000 stored whole), 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 0.1f 0x0122EE58, 0.5f 0x0122F59C,
//   255.0f 0x01230014, 15.0f 0x0124E640, 0.05f 0x0125A15C, 127.0f 0x0125A160, 1/30 0x012856DC, 2047.0f 0x01288BE4, 4095.0f 0x0129F668,
//   16383.0f 0x0129F66C, 1/99.9 0x0129F670, 10.0f 0x01237F0C; 5.0f 0x40A00000, 0.2f 0x3E4CCCCD stored whole.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPSCBASE_CTOR
#define FABLE_RETAIL_CPSCDECALRENDERER_CTOR
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTALIGNMENT
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#include "engine/CPSCDecalRenderer.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/GFPackFloat.h"

namespace NParticleEngine {

CPSCDecalRenderer::CPSCDecalRenderer()
    : DecalLifeSecs(1.0f), DecalBankIndex(0),
      StartColour(255, 255, 255, 255), MidColour(255, 255, 255, 255), EndColour(255, 255, 255, 255),
      FadeInEndInteger(0), FadeOutBeginInteger(1000),
      StartRenderSize(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), AlphaFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)),
      EndRenderSize(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), AnimationTimeSecs(GFPackFloat(1.0f, 14, 0.1f, 100.0f, 2)),
      SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)),
      UseStartColour(1), UseMidColour(0), UseEndColour(0),
      FlickerBias(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), FlickerMinAlphaInteger(0), FlickerMinSizeInteger(0),
      FlickerSpeed(GFPackFloat(15.0f, 12, 0.0f, 30.0f, 2)),
      AlphaFadeEnable(1), SizeFadeEnable(1), ForceAnimationTime(0), FlickerEnable(0), PoolingEnable(1),
      PoolIncreaseAlphaEnable(0), PoolIncreaseSizeEnable(1), PoolIncreaseLifeEnable(1), PoolIncreaseFrameEnable(1),
      MaxPoolSize(5.0f), MaxPoolAlpha(1.0f), MaxPoolLife(5.0f), PoolFrameIncreaseRate(1.0f), StencilCubeWidth(0.2f),
      LandscapeDecalsChildPrimIndex(-1), MeshDecalsChildPrimIndex(-1)
{
    Flags = GetParticleEmitterManager().GetDefaultAlignmentMode();
    BlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    BlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
}

} // namespace NParticleEngine