// Grade: DIFFER(673v681) -- x87 scheduling only: tangent-branch temp slots (0x30/0x1c v 0x24/0x10), Normalise squares z,y,x v retail y,z,x, h11*ToTangent keeps x on the stack v retail x,y stored; final result copy through [3c]
// NParticleEngine::CPSCOrbit::GetUpdateParam(C3DVector* position, long orbit, float t, bool tangent, CParticleUpdateParameters& params, long) -- one orbit's sample at t: the normalised tangent (linear blend of the from/to tangents) or the cubic Hermite position between the from/to positions and tangents
// Leaf: ?GetUpdateParam@CPSCOrbit@NParticleEngine
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16AA0, 688 bytes (`ret 18h`).
// Callees: virtual slot 8 (+0x20) on `this` (the update set-up, taking the parameters).
// Globals: epsilon 0x0129BA3C (1e-4f), the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Layout: retail CPSCBase (0xc): UpdateStarted bit 0 of +8; CPSCOrbit CentreParam +0x0c, Orbits[3] +0x10 (0x30 each),
//   OrbitStates[3] +0xa0 (COrbitState 0x44: FromPosition +8, ToPosition +0x14, NextPosition +0x20, FromTangent +0x2c,
//   ToTangent +0x38), as the landed COrbitState::Initialise 0x00B161A0.
struct CParticleUpdateParameters;
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)
extern unsigned long g_InvSqrtMantissaTable[128];    // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator*(float s, const C3DVector& v) { return C3DVector(s * v.X, s * v.Y, s * v.Z); }

struct COrbitSettings { unsigned char _body[0x30]; };
struct COrbitState {
    float     NextAngle;                    // +0x00
    float     RotateSpeedRandomValue;       // +0x04
    C3DVector FromPosition;                 // +0x08
    C3DVector ToPosition;                   // +0x14
    C3DVector NextPosition;                 // +0x20
    C3DVector FromTangent;                  // +0x2c
    C3DVector ToTangent;                    // +0x38
};

namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase {
    virtual ~CPSCBase();                                                    // slot 0
    virtual bool GetUpdateParam(C3DVector* position, long orbit, float t, bool tangent,
                                CParticleUpdateParameters& params, long unused);   // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void SetupUpdate(CParticleUpdateParameters& params);             // slot 8 (+0x20)
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};
struct CPSCOrbit : CPSCBase {
    unsigned long  CentreParam;             // +0x0c
    COrbitSettings Orbits[3];               // +0x10
    COrbitState    OrbitStates[3];          // +0xa0
    long           LifeTimer;               // +0x16c
    float          LifeTimeSecs;            // +0x170
    virtual bool GetUpdateParam(C3DVector* position, long orbit, float t, bool tangent,
                                CParticleUpdateParameters& params, long unused);
};

bool CPSCOrbit::GetUpdateParam(C3DVector* position, long orbit, float t, bool tangent,
                               CParticleUpdateParameters& params, long)
{
    if (!UpdateStarted)
        SetupUpdate(params);
    COrbitState* state;
    switch (orbit) {
    case 0: state = &OrbitStates[0]; break;
    case 1: state = &OrbitStates[1]; break;
    case 2: state = &OrbitStates[2]; break;
    default: return false;
    }
    if (tangent) {
        *position = state->FromTangent + (state->ToTangent - state->FromTangent) * t;
        if (position->GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            position->Normalise();
        return true;
    }
    float t2 = t * t;
    float t3 = t * t2;
    float twoT3 = t3 + t3;
    float threeT2 = t2 * 3.0f;
    float h00 = twoT3 - threeT2 + 1.0f;
    float h01 = threeT2 - twoT3;
    float h10 = t3 - (t2 + t2) + t;
    float h11 = t3 - t2;
    *position = h00 * state->FromPosition + h01 * state->ToPosition + h10 * state->FromTangent + h11 * state->ToTangent;
    return true;
}
}
