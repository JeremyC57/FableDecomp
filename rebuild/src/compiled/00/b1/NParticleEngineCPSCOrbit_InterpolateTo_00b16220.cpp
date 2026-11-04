// NParticleEngine::CPSCOrbit::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend the orbit component towards another one: the centre parameter lerped and rounded, then per orbit the type, enable and cycle flags taken from the target past the halfway point and the nine floats lerped
// Leaf: ?InterpolateTo@CPSCOrbit@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail-asm: 0x00B16269 `fistp dword ptr [esp+0Ch]` -- engine/GFFloatToLongNear.h (GFFloatToLongNear, inside the inline
//   NParticleEngine::GFInterpolate(unsigned long, unsigned long, float): the lerp stored to the dead `other` slot, reloaded, bare
//   dword fistp). VC7.1 C++ `(long)x` calls __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (NOTES.md).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B16220, length 370 bytes (padded to 384), `ret 8`.
// Identity: Ego_r.pdb ?InterpolateTo@CPSCOrbit@NParticleEngine@@UAEXABVCPSCBase@2@M@Z.
// Inlined: the NParticleEngine::GFInterpolate overloads of FableWin.pdb (KKKM unsigned long, _N_N0M bool, MMMM float) as the landed
//   CPSCLight::InterpolateTo 0x00B18C60 spells them.
// Globals: 1.0f @ 0x0122DED8, 2^32 @ 0x0122DCB4 (unsigned-to-float fix-up), 0.5f @ 0x0122F59C.
// Layout: Ego_r.pdb CPSCOrbit / COrbitSettings (the landed DatabaseCopy 0x00B163A0 model): CentreParam +0xc, Orbits[3] +0x10.
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"

namespace NParticleEngine {
inline float __fastcall GFInterpolate(float a, float b, float t) { return a * (1.0f - t) + b * t; }
inline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline bool __fastcall GFInterpolate(bool a, bool b, float t) { return t > 0.5f ? b : a; }
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
};

class CPSCOrbit : public CPSCBase { public:
    class COrbitSettings { public:                      // 48 bytes
        long  Type;
        bool  Enabled;
        float Radius;
        float Expand;
        bool  CycleFlag;
        float CycleTime;
        float SqueezeScale;
        float SqueezeAngle;
        float RotateSpeed;
        float RotateStart;
        float RotateSpeedRandom;
        float RotateStartRandom;
    };
    unsigned long  CentreParam;                         // +0x0c
    COrbitSettings Orbits[3];                           // +0x10
    virtual void InterpolateTo(const CPSCBase& other, float t);
};

void CPSCOrbit::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCOrbit& source = static_cast<const CPSCOrbit&>(other);
    CentreParam = GFInterpolate(CentreParam, source.CentreParam, t);
    for (long i = 0; i < 3; ++i) {
        Orbits[i].Type = t > 0.5f ? source.Orbits[i].Type : Orbits[i].Type;
        Orbits[i].Enabled = GFInterpolate(Orbits[i].Enabled, source.Orbits[i].Enabled, t);
        Orbits[i].Radius = GFInterpolate(Orbits[i].Radius, source.Orbits[i].Radius, t);
        Orbits[i].Expand = GFInterpolate(Orbits[i].Expand, source.Orbits[i].Expand, t);
        Orbits[i].CycleFlag = GFInterpolate(Orbits[i].CycleFlag, source.Orbits[i].CycleFlag, t);
        Orbits[i].CycleTime = GFInterpolate(Orbits[i].CycleTime, source.Orbits[i].CycleTime, t);
        Orbits[i].SqueezeScale = GFInterpolate(Orbits[i].SqueezeScale, source.Orbits[i].SqueezeScale, t);
        Orbits[i].SqueezeAngle = GFInterpolate(Orbits[i].SqueezeAngle, source.Orbits[i].SqueezeAngle, t);
        Orbits[i].RotateSpeed = GFInterpolate(Orbits[i].RotateSpeed, source.Orbits[i].RotateSpeed, t);
        Orbits[i].RotateStart = GFInterpolate(Orbits[i].RotateStart, source.Orbits[i].RotateStart, t);
        Orbits[i].RotateSpeedRandom = GFInterpolate(Orbits[i].RotateSpeedRandom, source.Orbits[i].RotateSpeedRandom, t);
        Orbits[i].RotateStartRandom = GFInterpolate(Orbits[i].RotateStartRandom, source.Orbits[i].RotateStartRandom, t);
    }
}

} // namespace NParticleEngine