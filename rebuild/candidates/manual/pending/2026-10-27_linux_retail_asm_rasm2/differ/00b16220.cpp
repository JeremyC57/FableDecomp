// NParticleEngine::CPSCOrbit::InterpolateTo(const CPSCOrbit& other, float t) -- blend this orbit component towards another: centre param and every float lerped by t, enum/bool settings switch to the other's past t = 0.5
// Leaf: InterpolateTo
// Retail-asm: 0x00B16265 `fld dword ptr [esp + 0x14]` ; 0x00B16269 `fistp dword ptr [esp + 0xc]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16220; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. CentreParam rounded by the helper through the dead argument slot.
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
namespace NParticleEngine {

struct COrbitSettings {                     // 48 bytes
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

struct CPSCOrbit {
    virtual ~CPSCOrbit();
    void*          ParticleSystem;          // +0x04
    unsigned long  Flags;                   // +0x08
    unsigned long  CentreParam;             // +0x0c
    COrbitSettings Orbits[3];               // +0x10
    void InterpolateTo(const CPSCOrbit& other, float t);
};

void CPSCOrbit::InterpolateTo(const CPSCOrbit& other, float t)
{
    float s = 1.0f - t;
    float centre = other.CentreParam * t + CentreParam * s;
    CentreParam = FloatToLong(centre);
    for (int i = 0; i < 3; ++i) {
        COrbitSettings& o = Orbits[i];
        const COrbitSettings& p = other.Orbits[i];
        o.Type = t > 0.5f ? p.Type : o.Type;
        o.Enabled = t > 0.5f ? p.Enabled : o.Enabled;
        o.Radius = p.Radius * t + o.Radius * s;
        o.Expand = p.Expand * t + o.Expand * s;
        o.CycleFlag = t > 0.5f ? p.CycleFlag : o.CycleFlag;
        o.CycleTime = p.CycleTime * t + o.CycleTime * s;
        o.SqueezeScale = p.SqueezeScale * t + o.SqueezeScale * s;
        o.SqueezeAngle = p.SqueezeAngle * t + o.SqueezeAngle * s;
        o.RotateSpeed = p.RotateSpeed * t + o.RotateSpeed * s;
        o.RotateStart = p.RotateStart * t + o.RotateStart * s;
        o.RotateSpeedRandom = p.RotateSpeedRandom * t + o.RotateSpeedRandom * s;
        o.RotateStartRandom = p.RotateStartRandom * t + o.RotateStartRandom * s;
    }
}

} // namespace NParticleEngine
