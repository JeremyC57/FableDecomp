// NParticleEngine::CParticleEmitter::InterpolateTo(const CParticleEmitter& other, float t) -- blend an emitter towards another one: each particle system towards its partner, the flags and distances lerped (bits rounded to nearest), Priority and the 2D layer taken from the target past the halfway point
// Leaf: ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AED600, 1254 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z (public, non-virtual); called by
//   CParticleEmitterManager::CreateInterpolatedEmitter (0x00AEB674); `ret 8` = (const CParticleEmitter&, float).
// Callees: CParticleSystem::InterpolateTo(const CParticleSystem&, float) 0x00AF4100 (Ego_r.pdb / FableWin.pdb
//   ?InterpolateTo@CParticleSystem@NParticleEngine@@QAEXABV12@M@Z; its only caller is this row, 0x00AED623). Floats 1.0 0x0122DED8,
//   0.5 0x0122F59C.
// Layout: retail CParticleEmitter (engine/CParticleEmitter.h FABLE_RETAIL_CPARTICLEEMITTER: ParticleSystems +0x08, the unsigned short
//   flag word +0x40 (Priority:3 bits 0-2, IsScreenDisplacement 3, ReadZBuffer 4, ContinuousEmitter 7, Emitter2D 11), Emitter2DLayer:7
//   +0x42, the unsigned char flag bytes +0x44 / +0x45, the distances +0x158..+0x168).
// Shape: FableWin.pdb NParticleEngine::GFInterpolate overloads (`YIGGGM` unsigned short, `YIEEEM` unsigned char, `YIMMMM` float) inline:
//   (1 - t) * a + b * t, the integer ones rounded with GFFloatToLongNear (the bare `fstp ; fld ; fistp dword` sites).
// Retail-asm: 0x00AED67B `fistp dword ptr [esp+14h]` (and every later bit rounding of this body) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`, so no C++ form gives the bare dword
//   fistp (rasm1, GOTCHAS; the landed CPSCRenderSprite::InterpolateTo 0x00AF9840 uses the same helper).
// Helper: the shared engine/GFFloatToLongNear.h (asm path + FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATETO
#define FABLE_RETAIL_CPARTICLESYSTEM_INTERPOLATETO
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CParticleEmitter.h"
#include "engine/CParticleSystem.h"
#include "engine/GFFloatToLongNear.h"
namespace NParticleEngine {
inline unsigned short __fastcall GFInterpolate(unsigned short a, unsigned short b, float t) { return (unsigned short)GFFloatToLongNear((1.0f - t) * a + b * t); }
inline unsigned char __fastcall GFInterpolate(unsigned char a, unsigned char b, float t) { return (unsigned char)GFFloatToLongNear((1.0f - t) * a + b * t); }
inline float __fastcall GFInterpolate(float a, float b, float t) { return (1.0f - t) * a + b * t; }
inline long __fastcall GFInterpolateAsFlags(long a, long b, float t) { return t > 0.5f ? b : a; }

void CParticleEmitter::InterpolateTo(const CParticleEmitter& other, float t)
{
    std::vector<CCountedPointer<CParticleSystem> >::const_iterator source = other.ParticleSystems.begin();
    for (std::vector<CCountedPointer<CParticleSystem> >::iterator it = ParticleSystems.begin(); it != ParticleSystems.end(); ++it, ++source)
        (*it)->InterpolateTo(**source, t);
    ContinuousEmitter = GFInterpolate(ContinuousEmitter, other.ContinuousEmitter, t);
    MaxSpawnDistance = GFInterpolate(MaxSpawnDistance, other.MaxSpawnDistance, t);
    MaxDrawDistance = GFInterpolate(MaxDrawDistance, other.MaxDrawDistance, t);
    FadeOutStart = GFInterpolate(FadeOutStart, other.FadeOutStart, t);
    FadeInEnd = GFInterpolate(FadeInEnd, other.FadeInEnd, t);
    FadeInStart = GFInterpolate(FadeInStart, other.FadeInStart, t);
    Priority = GFInterpolateAsFlags(Priority, other.Priority, t);
    IsScreenDisplacement = GFInterpolate(IsScreenDisplacement, other.IsScreenDisplacement, t);
    ReadZBuffer = GFInterpolate(ReadZBuffer, other.ReadZBuffer, t);
    Emitter2D = GFInterpolate(Emitter2D, other.Emitter2D, t);
    Emitter2DLayer = GFInterpolateAsFlags(Emitter2DLayer, other.Emitter2DLayer, t);
    PreWaterEffect = GFInterpolate(PreWaterEffect, other.PreWaterEffect, t);
    WaterEffect = GFInterpolate(WaterEffect, other.WaterEffect, t);
    ZBufferWriteable = GFInterpolate(ZBufferWriteable, other.ZBufferWriteable, t);
    DieIfOffsceen = GFInterpolate(DieIfOffsceen, other.DieIfOffsceen, t);
    OffscreenUpdate = GFInterpolate(OffscreenUpdate, other.OffscreenUpdate, t);
    ClipEffectToWeatherMask = GFInterpolate(ClipEffectToWeatherMask, other.ClipEffectToWeatherMask, t);
    EnableDithering = GFInterpolate(EnableDithering, other.EnableDithering, t);
    CalcBoundingSphereOnceOnly = GFInterpolate(CalcBoundingSphereOnceOnly, other.CalcBoundingSphereOnceOnly, t);
}
}