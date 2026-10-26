// NParticleEngine::CParticle::GetInterpolatedLifetimerInSeconds(float) -- the particle's age in seconds: (Lifetimer - CreationTime/255 + fraction) times the global update period
// Leaf: GetInterpolatedLifetimerInSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF85E0, length 49 bytes (`ret 4`).
// Callees: the update-period getter @ 0x00AEAA00 (`fld [0x01399508] ; ret`, landed as
//          global_GetMaxDrawDistance_00aeaa00 under a propagated label); float pool 0x01231724 =
//          0.0039215689f (1/255).
// Layout (PDB NParticleEngine::CParticle): Lifetimer short +0x72, CreationTime uchar +0x74.
// Shape: the getter is called first and its result stays in st(0) for the final `fmulp`; both
// integer members are loaded, then converted through the one `push ecx` frame slot (`fild`); the
// creation time is scaled by the literal 1/255 (`fmul [pool]`, the member loaded first), subtracted
// from the lifetimer, the fraction argument added last.

float GetParticleUpdatePeriod();                                          // 0x00AEAA00

namespace NParticleEngine {

class CParticle { public:
    virtual void v00();
    unsigned char _state[0x6e];                                           // +0x04 Orientation .. Acceleration
    short         Lifetimer;                                              // +0x72
    unsigned char CreationTime;                                           // +0x74
    float GetInterpolatedLifetimerInSeconds(float fraction) const;
};

float CParticle::GetInterpolatedLifetimerInSeconds(float fraction) const
{
    return GetParticleUpdatePeriod() * (Lifetimer - CreationTime * (1.0f / 255.0f) + fraction);
}

} // namespace NParticleEngine