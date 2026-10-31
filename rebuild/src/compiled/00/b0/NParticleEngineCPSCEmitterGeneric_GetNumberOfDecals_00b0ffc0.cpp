// NParticleEngine::CPSCEmitterGeneric::GetNumberOfDecals(float& count, float time) -- the number of decals this emitter will produce: rate x time (or x the emitter's life span), at least the start count; FLT_MAX and false for an unbounded emitter
// Leaf: ?GetNumberOfDecals@CPSCEmitterGeneric@NParticleEngine@@UAE_NAAMM@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B10006 `fld dword ptr [esp]` + 0x00B10009 `fistp dword ptr [esp+4]` -- the engine's inline-assembly float-to-long helper (landed FloatToLong, CPSCEmitterGeneric::GetRandomRadius 0x00B102C0); VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0FFC0, 605 bytes, `ret 8` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb + FableWin.pdb ?GetNumberOfDecals@CPSCEmitterGeneric@NParticleEngine@@UAE_NAAMM@Z (virtual, float& count, float time)
//   + the manifest row (CPSCEmitterGeneric::GetNumberOfDecals). Layout: Ego_r.pdb bitfields (+0x30 NoParticlesToStart /
//   NoParticlesToStartRand / UseEmitterLifeSecs bit 23 / UseEmitterTimelineSecs bit 24; +0x34 ParticlesPerSecond bit 18;
//   +0x40 EmitterLifeSecs / EmitterStartTime).
// Constants: 1/16383 0x0129F664, 100.0f 0x01230A08, 1/32767 0x0129C81C, 300.0f 0x01238808, 10.0f 0x01237F0C, 1.0f 0x0122DED8,
//   0.0f 0x0122DEDC, 2^32 0x0122DCB4, FLT_MAX immediate 0x7F7FFFFF. Shape: the max is the macro form (the start count is
//   re-read from the bitfields when it wins); the pps quantiser runs before the branch.
namespace NParticleEngine {

class CPSCBase { public:
    virtual ~CPSCBase();
    void*         ParticleSystem;               // +0x04 CParticleSystem*
    bool          UpdateStarted : 1;            // +0x08 bit 0
    bool          Initialised : 1;              // +0x08 bit 1
};

class CPSCEmitterGeneric : public CPSCBase { public:
    unsigned long EmitterPosParamDesc[3];       // +0x0c CParticleUpdateParamDesc<unsigned long,unsigned long>
    void*         Spline[2];                    // +0x18 CCountedPointer<CCardinalSpline>
    unsigned long EmitterPosParam;              // +0x20
    unsigned long DirectionParamName;           // +0x24
    long          LifeTimer;                    // +0x28
    float         NextGenerateTime;             // +0x2c
    unsigned long NoParticlesToStart : 10;      // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;       // bit 23
    unsigned long UseEmitterTimelineSecs : 1;   // bit 24
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;  // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;      // bit 18
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
    virtual bool GetNumberOfParticles(float& count, float time);
    virtual bool GetNumberOfDecals(float& count, float time);
};

// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
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

bool CPSCEmitterGeneric::GetNumberOfDecals(float& count, float time)
{
    float unit = (float)ParticlesPerSecond * (1.0f / 16383.0f);
    float rate = RoundToDecimals(unit * 100.0f, 2);
    if (time > 0.0f) {
        float n = rate * time;
        count = n > (float)(NoParticlesToStartRand + NoParticlesToStart) ? n : (float)(NoParticlesToStartRand + NoParticlesToStart);
        return true;
    }
    if (UseEmitterLifeSecs && !UseEmitterTimelineSecs) {
        float lifeUnit = (float)EmitterLifeSecs * (1.0f / 32767.0f);
        float life = RoundToDecimals(lifeUnit * 300.0f, 2);
        float startUnit = (float)EmitterStartTime * (1.0f / 32767.0f);
        float start = RoundToDecimals(startUnit * 300.0f, 2);
        float n = (life - start) * rate;
        count = n > (float)(NoParticlesToStartRand + NoParticlesToStart) ? n : (float)(NoParticlesToStartRand + NoParticlesToStart);
        return true;
    }
    if (rate <= 0.0f) {
        count = (float)(NoParticlesToStartRand + NoParticlesToStart);
        return true;
    }
    count = 3.402823466e+38f;
    return false;
}

} // namespace NParticleEngine