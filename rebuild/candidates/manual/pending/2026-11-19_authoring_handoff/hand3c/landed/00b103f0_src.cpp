// NParticleEngine::CPSCEmitterGeneric::CPSCEmitterGeneric() -- default generic emitter component: an empty position-parameter descriptor and spline, zero timers, solid XY-oriented emission along a custom direction (0, 0, 1), 3 particles per second, size 1, a 10 s life on a 30 s timeline, unit non-uniform scaling
// Leaf: ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B10457 `fld dword ptr [esp+8]` + 0x00B1045B `fistp dword ptr [esp+0Ch]` (34 sites) -- the engine's inline-assembly
//   GFFloatToLongNear inside the shared engine/GFPackFloat.h quantiser (round to two decimals, then pack); no asm in this row.
//   VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B103F0, 1463 bytes, plain `ret` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb + FableWin.pdb ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ + the manifest row; vtable 0x012A002C.
// Callees: CPSCBase::CPSCBase() 0x00AF2620, CBaseClass() 0x0099A2D0 (the CParticleUpdateParamDesc member, vtable 0x0125FEE4).
// Layout: Ego_r.pdb CPSCEmitterGeneric (80 bytes; bit fields +0x30..+0x4c as GetNumberOfDecals 0x00B0FFC0 plus MaxSpeed /
//   NonUniformScalingX/Y +0x44, NonUniformScalingZ ushort +0x48, StartedTimer / FinishedGenerating +0x4c); EmitterPosParam (+0x20)
//   and the descriptor's Data are not initialised.
// Shape: every member, the bit fields included, is in the mem-initialiser list (the object members come first). Each packed float
//   is the engine's GFPackFloat(value, bits, min, max, 2) (FableWin.pdb ?GFPackFloat@@YIKMKMMK@Z: the /Od ctor 0x02F47CE0 calls it
//   with ecx = bits, edx = 2, in member order), the shared engine/GFPackFloat.h with its FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
//   body (as the landed CPSCEmitterGeneric::DatabaseSerialise 0x00B13530): that closes the NonUniformScalingY register swap
//   (0x00B10923) the local pack helper left. Header opt-ins (hand3c overlay): FABLE_RETAIL_CPSCBASE_CTOR (CPSCBase() 0x00AF2620),
//   FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_CTOR (the inline descriptor ctor), FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR (declaration).
// Constants: 10.0f 0x01237F0C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 0.1f 0x0122EE58, 0.5f 0x0122F59C, 255.0f 0x01230014,
//   15.0f 0x0124E640, 0.05f 0x0125A15C, 127.0f 0x0125A160, 1/30 0x012856DC, 2047.0f 0x01288BE4, 1023.0f 0x0129EF90, 4095.0f 0x0129F668,
//   16383.0f 0x0129F66C, 1/99.9 0x0129F670 (read from the binary).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPSCBASE_CTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#include "engine/CPSCEmitterGeneric.h"
#include "engine/GFPackFloat.h"

namespace NParticleEngine {

CPSCEmitterGeneric::CPSCEmitterGeneric()
    : DirectionParamName(0), LifeTimer(0), NextGenerateTime(0.0f),
      NoParticlesToStart(0), NoParticlesToStartRand(0), EmitterType(0), Solid(1), UseEmitterLifeSecs(0), UseEmitterTimelineSecs(0),
      OrientationXY(1), OrientationXZ(0), OrientationYZ(0), UseCustomDirection(1), UseOutwardDirection(0), UseParamDirection(0),
      OppositeDirection(0),
      AngularPerturbationInteger(0), CustomDirectionX(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)),
      ParticlesPerSecond(GFPackFloat(3.0f, 14, 0.0f, 100.0f, 2)),
      EmitterSize(GFPackFloat(1.0f, 10, 0.0f, 10.0f, 2)), RadialBias(GFPackFloat(0.0f, 10, -5.0f, 5.0f, 2)),
      MinSpeed(GFPackFloat(0.0f, 10, 0.0f, 10.0f, 2)), UseForwardDirection(0), UseRandom2DDirection(0),
      CustomDirectionY(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), CustomDirectionZ(GFPackFloat(1.0f, 8, -1.0f, 1.0f, 2)),
      EmitterTimelineSecs(GFPackFloat(30.0f, 15, 0.0f, 300.0f, 2)), UseRandom3DDirection(0),
      EmitterLifeSecs(GFPackFloat(10.0f, 15, 0.0f, 300.0f, 2)), EmitterStartTime(GFPackFloat(0.0f, 15, 0.0f, 300.0f, 2)),
      MaxSpeed(GFPackFloat(0.0f, 10, 0.0f, 10.0f, 2)), NonUniformScalingX(GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
      NonUniformScalingY(GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
      NonUniformScalingZ(GFPackFloat(1.0f, 11, -10.0f, 10.0f, 2)),
      StartedTimer(0), FinishedGenerating(0)
{
}

} // namespace NParticleEngine
