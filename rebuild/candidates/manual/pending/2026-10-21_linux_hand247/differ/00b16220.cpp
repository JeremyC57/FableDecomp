// Grade: DIFFER(334v372) -- retail rounds CentreParam with a bare dword fistp through the dead argument slot (inline-asm helper, lane gap per GOTCHAS chunk 145); the loop shape was not pursued further
// NParticleEngine::CPSCOrbit::InterpolateTo(const CPSCOrbit& other, float t) -- blend this orbit component towards another: centre param and every float lerped by t, enum/bool settings switch to the other's past t = 0.5
// Leaf: InterpolateTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B16220, length 370 bytes (padded to 384), `ret 8`.
// Globals: 1.0f @ 0x0122DED8, 2^32 @ 0x0122DCB4 (unsigned-to-float fix-up), 0.5f @ 0x0122F59C.
// Layout: landed CPSCOrbit model (WriteBinary 0x00B163E0): CentreParam +0xc, COrbitSettings Orbits[3] +0x10 (0x30 each).
// Shape: s = 1 - t; CentreParam = round(other.CentreParam * t + CentreParam * s) through a float in the dead `other` slot and
//   a bare dword `fistp` (retail's inline-asm rounding helper); each orbit picks Type/Enabled/CycleFlag by t > 0.5 and lerps
//   the nine floats as other * t + this * s.
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
    CentreParam = (long)centre;
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
