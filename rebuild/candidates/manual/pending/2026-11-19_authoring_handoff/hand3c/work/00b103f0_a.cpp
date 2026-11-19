// NParticleEngine::CPSCEmitterGeneric::CPSCEmitterGeneric() -- default generic emitter component: an empty position-parameter descriptor and spline, zero timers, solid XY-oriented emission along a custom direction (0, 0, 1), 3 particles per second, size 1, a 10 s life on a 30 s timeline, unit non-uniform scaling
// Grade: DIFFER(1464v1463) 78/365 -- one 4-instruction register swap (1 byte: `and eax` short form): the top field NonUniformScalingY (+0x44 bits 21-31) is stored as `(old & 1FFFFFh) | (v << 21)` in retail (old in eax) but `(v << 21) | (old & mask)` here; mem-initialiser vs body assignment, signed pack and assignment-order levers inert; the other lines are the vptr/constant relocations
// Leaf: ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B10457 `fld dword ptr [esp+8]` + 0x00B1045B `fistp dword ptr [esp+0Ch]` -- the engine's inline-assembly float-to-long helper (landed FloatToLong, CPSCEmitterGeneric::GetRandomRadius 0x00B102C0), 34 sites (round to two decimals, then pack); VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B103F0, 1463 bytes, plain `ret` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb + FableWin.pdb ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ + the manifest row; vtable 0x012A002C.
// Callees: CPSCBase::CPSCBase() 0x00AF2620, CBaseClass() 0x0099A2D0 (the CParticleUpdateParamDesc member, vtable 0x0125FEE4).
// Layout: Ego_r.pdb CPSCEmitterGeneric (80 bytes; bit fields +0x30..+0x4c as GetNumberOfDecals 0x00B0FFC0 plus MaxSpeed /
//   NonUniformScalingX/Y +0x44, NonUniformScalingZ ushort +0x48, StartedTimer / FinishedGenerating +0x4c); EmitterPosParam (+0x20)
//   and the descriptor's Data are not initialised.
// Shape: every member, the bit fields included, is in the mem-initialiser list (the object members come first). The pack is round-to-2-decimals, `(r - min) * (1 / (max - min))` as one statement,
//   then `* steps` into FloatToLong (one expression lets VC7.1 fold the two constants into one multiply). PackParam is a local
//   helper for that database macro shape (unproven spelling; fully inlined, no symbol).
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
