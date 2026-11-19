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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0
#include "engine/CCountedPointer.h"
class CCardinalSpline;

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CEngineInternalPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CEngineInternalPrimitiveBase* Data;              // +0x04
    CEnginePrimitiveHandle() : Data(0) { }
};

static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}
// the database parameter packing: round to two decimals, map [min, max] onto [0, steps]
static __forceinline unsigned long PackParam(float value, float min, float max, float steps)
{
    float normalised = (RoundToDecimals(value, 2) - min) * (1.0f / (max - min));
    return FloatToLong(normalised * steps);
}

namespace NParticleEngine {

class CParticleEmitterManager { public:
    const long GetDefaultBlendMode() const { return 2; }        // FableWin.pdb ?GetDefaultBlendMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ (inline)
    const long GetDefaultBlendOp() const { return 0; }          // FableWin.pdb ?GetDefaultBlendOp@...@@QBE?BJXZ (inline)
    const long GetDefaultAlignmentMode() const { return 0; }    // FableWin.pdb ?GetDefaultAlignmentMode@...@@QBE?BJXZ (inline)
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0

class CPSCBase { public:
    CPSCBase();                                      // 0x00AF2620
    virtual ~CPSCBase();
    void*         ParticleSystem;               // +0x04
    bool          UpdateStarted : 1;            // +0x08
    bool          Initialised : 1;
};

template <class A, class B> class CParticleUpdateParamCallback;
template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4
public:
    CParticleUpdateParamCallback<A, B>* Callback;           // +0x04
    unsigned long                       Data;               // +0x08
    CParticleUpdateParamDesc() : Callback(0) { }
};

class CPSCEmitterGeneric : public CPSCBase { public:
    CParticleUpdateParamDesc<unsigned long, unsigned long> EmitterPosParamDesc;   // +0x0c
    CCountedPointer<CCardinalSpline> Spline;    // +0x18
    unsigned long EmitterPosParam;              // +0x20
    unsigned long DirectionParamName;           // +0x24
    long          LifeTimer;                    // +0x28
    float         NextGenerateTime;             // +0x2c
    unsigned long NoParticlesToStart : 10;      // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;  // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;             // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;         // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;         // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;     // +0x48
    unsigned long StartedTimer : 1;             // +0x4c
    unsigned long FinishedGenerating : 1;
    CPSCEmitterGeneric();
};

CPSCEmitterGeneric::CPSCEmitterGeneric()
    : DirectionParamName(0), LifeTimer(0), NextGenerateTime(0.0f),
      NoParticlesToStart(0), NoParticlesToStartRand(0), EmitterType(0), Solid(1), UseEmitterLifeSecs(0), UseEmitterTimelineSecs(0),
      OrientationXY(1), OrientationXZ(0), OrientationYZ(0), UseCustomDirection(1), UseOutwardDirection(0), UseParamDirection(0),
      OppositeDirection(0),
      AngularPerturbationInteger(0), CustomDirectionX(PackParam(0.0f, -1.0f, 1.0f, 255.0f)),
      ParticlesPerSecond(PackParam(3.0f, 0.0f, 100.0f, 16383.0f)),
      EmitterSize(PackParam(1.0f, 0.0f, 10.0f, 1023.0f)), RadialBias(PackParam(0.0f, -5.0f, 5.0f, 1023.0f)),
      MinSpeed(PackParam(0.0f, 0.0f, 10.0f, 1023.0f)), UseForwardDirection(0), UseRandom2DDirection(0),
      CustomDirectionY(PackParam(0.0f, -1.0f, 1.0f, 255.0f)), CustomDirectionZ(PackParam(1.0f, -1.0f, 1.0f, 255.0f)),
      EmitterTimelineSecs(PackParam(30.0f, 0.0f, 300.0f, 32767.0f)), UseRandom3DDirection(0),
      EmitterLifeSecs(PackParam(10.0f, 0.0f, 300.0f, 32767.0f)), EmitterStartTime(PackParam(0.0f, 0.0f, 300.0f, 32767.0f)),
      MaxSpeed(PackParam(0.0f, 0.0f, 10.0f, 1023.0f)), NonUniformScalingX(PackParam(1.0f, -10.0f, 10.0f, 2047.0f)),
      NonUniformScalingY(PackParam(1.0f, -10.0f, 10.0f, 2047.0f)),
      NonUniformScalingZ(PackParam(1.0f, -10.0f, 10.0f, 2047.0f)),
      StartedTimer(0), FinishedGenerating(0)
{
}

} // namespace NParticleEngine
