// Grade: DIFFER(372v372) -- CentreParam through the FloatToLong retail_asm helper, the orbit loop indexes Orbits[i] / other.Orbits[i] directly (retail's two pointer IVs and delta register); residue: [edx+edi] v [edi+edx] in the Type select and the Radius lerp's operand order
// NParticleEngine::CPSCOrbit::InterpolateTo(const CPSCOrbit& other, float t) -- blend this orbit component towards another: centre param and every float lerped by t, enum/bool settings switch to the other's past t = 0.5
// Retail-asm: 0x00B16265 fld dword ptr [esp+14h] ; fistp dword ptr [esp+0Ch] -- the engine FloatToLong helper
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

__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

void CPSCOrbit::InterpolateTo(const CPSCOrbit& other, float t)
{
    float s = 1.0f - t;
    float centre = other.CentreParam * t + CentreParam * s;
    CentreParam = FloatToLong(centre);
    for (int i = 0; i < 3; ++i) {
        Orbits[i].Type = t > 0.5f ? other.Orbits[i].Type : Orbits[i].Type;
        Orbits[i].Enabled = t > 0.5f ? other.Orbits[i].Enabled : Orbits[i].Enabled;
        Orbits[i].Radius = other.Orbits[i].Radius * t + Orbits[i].Radius * s;
        Orbits[i].Expand = other.Orbits[i].Expand * t + Orbits[i].Expand * s;
        Orbits[i].CycleFlag = t > 0.5f ? other.Orbits[i].CycleFlag : Orbits[i].CycleFlag;
        Orbits[i].CycleTime = other.Orbits[i].CycleTime * t + Orbits[i].CycleTime * s;
        Orbits[i].SqueezeScale = other.Orbits[i].SqueezeScale * t + Orbits[i].SqueezeScale * s;
        Orbits[i].SqueezeAngle = other.Orbits[i].SqueezeAngle * t + Orbits[i].SqueezeAngle * s;
        Orbits[i].RotateSpeed = other.Orbits[i].RotateSpeed * t + Orbits[i].RotateSpeed * s;
        Orbits[i].RotateStart = other.Orbits[i].RotateStart * t + Orbits[i].RotateStart * s;
        Orbits[i].RotateSpeedRandom = other.Orbits[i].RotateSpeedRandom * t + Orbits[i].RotateSpeedRandom * s;
        Orbits[i].RotateStartRandom = other.Orbits[i].RotateStartRandom * t + Orbits[i].RotateStartRandom * s;
    }
}

} // namespace NParticleEngine
