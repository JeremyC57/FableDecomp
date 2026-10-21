// NParticleEngine::CPSCLight::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- grow the system's bounding sphere by the light: its current position (moved into system space unless the system is local) with the radius interpolated over the light's life
// Leaf: AddToBoundingSphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B18AF0, length 356 bytes (padded to 368), `ret 10h`.
// Ego_r.pdb: `?AddToBoundingSphere@CPSCLight@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z`.
// Callees: CPSCBase::GetCurrentPosition(params, const unsigned long& param, float, bool, unsigned long) const @ 0x00AF2640
//          (hidden C3DVector result), CParticleSystem::GetInverseSystemTransform(CMatrix3x4&) @ 0x00AF2250, the particle frame
//          rate getter @ 0x00AEA9F0 (returns the long @ 0x01399504; manifest "GFGetPreMainMemoryUsed" is a byte twin),
//          CParticleSystem::ScaleParticleDimensions(float&) @ 0x00AF8F80, C3DBoundingSphere::AddToBounds(const C3DVector&, float)
//          @ 0x00ADDD80.
// Globals: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, epsilon 0.0001f @ 0x0129BA3C.
// Layout: PDB CPSCLight (0x70), retail offsets equal: LightLifeTimer +0xc, LightIsAlive +0x14, LightPositionParam +0x18,
//   LightLifeSecs +0x1c, LightStartRenderWorldRadius +0x2c, LightEndRenderWorldRadius +0x30, flag bits +0x48 (bit 1
//   LightUseLifeSecs). CParticleSystem: the local-space flag is bit 6 of +0x50.
// Shape: a single `if (LightIsAlive) {...}` keeps `return false` at the end; the life fraction defaults to 0 and is clamped to
//   [0, 1 - epsilon]; the radius comes from an inline Lerp(start, end, t) = (end - start) * t + start (start kept in a register)
//   and is written back into the dead timeOffset parameter (retail's [esp+4Ch] slot, shared with the clamp limit; a new local
//   takes the params slot and also turns the flag test into `test byte ptr`); each transformed component is an accumulated
//   chain in retail's term order (x and y rows z, x, y; the z row z, y, x).
extern float g_ParticleEpsilon;                 // 0x0129BA3C

struct C3DVector {
    float X, Y, Z;
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

struct C3DBoundingSphere {
    void AddToBounds(const C3DVector& point, float radius);     // 0x00ADDD80
};

template <class A, class B> struct CParticleUpdateParameters;

inline float Lerp(float a, float b, float t) { return (b - a) * t + a; }

long GetParticleFrameRate();                    // 0x00AEA9F0

namespace NParticleEngine {

struct CParticleSystem {
    unsigned char _pad00[0x50];
    unsigned char Flags;                        // +0x50 (bit 6: local space)
    void GetInverseSystemTransform(CMatrix3x4& matrix);         // 0x00AF2250
    void ScaleParticleDimensions(float& size);                  // 0x00AF8F80
};

struct CPSCBase {
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                 const unsigned long& param, float timeOffset, bool local, unsigned long flags) const;  // 0x00AF2640
};

struct CPSCLight : CPSCBase {
    long          LightLifeTimer;               // +0x0c
    long          LightRespawnDelayTimer;       // +0x10
    bool          LightIsAlive;                 // +0x14
    unsigned long LightPositionParam;           // +0x18
    float         LightLifeSecs;                // +0x1c
    float         LightRespawnDelaySecs;        // +0x20
    float         LightStartTime;               // +0x24
    float         LightTimelineSecs;            // +0x28
    float         LightStartRenderWorldRadius;  // +0x2c
    float         LightEndRenderWorldRadius;    // +0x30
    long          LightAttenuationFactor;       // +0x34
    long          LightFadeInEndInteger;        // +0x38
    long          LightFadeOutBeginInteger;     // +0x3c
    float         LightWorldRadiusFadeMinimum;  // +0x40
    unsigned long LightColourFadeMinimum;       // +0x44
    bool          LightWorldRadiusFadeEnable : 1;   // +0x48 bit 0
    bool          LightUseLifeSecs : 1;             //        bit 1
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system,
                                     const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};

bool CPSCLight::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system,
                                    const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    if (LightIsAlive) {
        C3DVector pos = GetCurrentPosition(params, LightPositionParam, timeOffset, false, 0);
        if (!(system.Flags & 0x40)) {
            CMatrix3x4 m;
            system.GetInverseSystemTransform(m);
            float x = pos.X;
            float y = pos.Y;
            float z = pos.Z;
            pos.X = m.E31 * z;
            pos.X += m.E11 * x;
            pos.X += m.E21 * y;
            pos.X += m.E41;
            pos.Y = m.E32 * z;
            pos.Y += m.E12 * x;
            pos.Y += m.E22 * y;
            pos.Y += m.E42;
            pos.Z = m.E33 * z;
            pos.Z += m.E23 * y;
            pos.Z += m.E13 * x;
            pos.Z += m.E43;
        }
        float frameRate = (float)GetParticleFrameRate();
        float t = 0.0f;
        if (LightUseLifeSecs) {
            t = ((float)LightLifeTimer + timeOffset) / (frameRate * LightLifeSecs);
            if (t < 0.0f)
                t = 0.0f;
            else {
                float limit = 1.0f - g_ParticleEpsilon;
                if (t > limit)
                    t = limit;
            }
        }
        timeOffset = Lerp(LightStartRenderWorldRadius, LightEndRenderWorldRadius, t);
        system.ScaleParticleDimensions(timeOffset);
        sphere.AddToBounds(pos, timeOffset);
        return true;
    }
    return false;
}

}