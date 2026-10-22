// Grade: DIFFER(787v787) -- A3 levers applied (Dot member, G/B sum temp, lambert Y,X,Z, FastInvSqrt *=): left = Normalise reciprocal spill (fstp [r]; fmul [r]) and colour R/G spilled to dead arg slots esp+8/+0xc (ours +0x18)
// CEngineLightingManager::CalcSWLighting(const C3DVector& position, const C3DVector& normal, const CEngineLocalLightArray& lights) -- software lighting of one vertex: the global diffuse / ambient / backlight terms as the global-only overload 0x00B45510, plus every local light (fast-normalised direction, clamped Lambert term, inverse-square attenuation) when its Lambert term passes 0.0001; RGB clamped to 1, alpha 1
// Leaf: CalcSWLighting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B45720, length 785 bytes (`ret 10h`: hidden result, position, normal, lights). No callees.
// Floats: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C, 0.0001f @ 0x0129BA3C; the reciprocal square
//   root mantissa table @ 0x013CE558 (the engine's inline Normalise, as the landed CTCPhysicsRigidBody::GetRHSet 0x00759B40).
// Layout: CEngineLightingManager retail offsets as 0x00B45510; CEngineLocalLightArray / CEngineLocalLightElement from Ego_r.pdb
//   (LightCount +0, Lights[8] +4 of 0x34: WorldPos, Colour +0x18, AttenuationDistSquared +0x28, AttenuationConstant +0x2c).
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
inline float Sqr(float x) { return x * x; }
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { float d = Y * o.Y; d += Z * o.Z; d += o.X * X; return d; }
    float LengthSquared() const { return Z * Z + Y * Y + X * X; }
    C3DVector& operator*=(const float& s) { Y *= s; X *= s; Z *= s; return *this; }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        *this *= FastInvSqrt(lengthSquared);
    }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour& operator+=(const CRGBFloatColour& c) { R += c.R; G += c.G; B += c.B; return *this; }
    __forceinline void Clamp()
    {
        if (R < 0.0f) R = 0.0f; else if (R > 1.0f) R = 1.0f;
        if (G < 0.0f) G = 0.0f; else if (G > 1.0f) G = 1.0f;
        if (B < 0.0f) B = 0.0f; else if (B > 1.0f) B = 1.0f;
        if (A < 0.0f) A = 0.0f; else if (A > 1.0f) A = 1.0f;
    }
};

struct CEngineLocalLightElement {                  // 0x34 bytes
    C3DVector       WorldPos;                      // +0x00
    float           Radius;                        // +0x0c
    float           RadiusSquared;                 // +0x10
    float           SpotRadius;                    // +0x14
    CRGBFloatColour Colour;                        // +0x18
    float           AttenuationDistSquared;        // +0x28
    float           AttenuationConstant;           // +0x2c
    unsigned long   ChannelMask;                   // +0x30
};
struct CEngineLocalLightArray {
    long                     LightCount;           // +0x00
    CEngineLocalLightElement Lights[8];            // +0x04
};
inline float Clamp01(float x)
{
    if (x < 0.0f)
        x = 0.0f;
    else if (x > 1.0f)
        x = 1.0f;
    return x;
}

struct CEngineLightingManager {
    unsigned char   _pad00[0x30];
    C3DVector       DiffuseVector;                 // +0x30
    unsigned char   _pad3c[0xc];                   // +0x3c ThemeChannels (12-byte vector)
    CRGBFloatColour PrimitiveDiffuseColour;        // +0x48
    CRGBFloatColour PrimitiveAmbientColour;        // +0x58
    CRGBFloatColour PrimitiveBacklightColour;      // +0x68
    CRGBFloatColour CalcSWLighting(const C3DVector& position, const C3DVector& normal, const CEngineLocalLightArray& lights);
};

CRGBFloatColour CEngineLightingManager::CalcSWLighting(const C3DVector& position, const C3DVector& normal, const CEngineLocalLightArray& lights)
{
    float facing = -DiffuseVector.Dot(normal);
    float diffuse = MIN(Sqr(MAX(facing, 0.0f)), 1.0f);
    float back = MAX(-facing, 0.0f);
    CRGBFloatColour sum = PrimitiveDiffuseColour * diffuse + PrimitiveAmbientColour + PrimitiveBacklightColour * back;
    CRGBFloatColour colour(sum.R, sum.G, sum.B, sum.A);
    for (long i = 0; i < lights.LightCount; ++i) {
        const CEngineLocalLightElement& light = lights.Lights[i];
        C3DVector direction = light.WorldPos - position;
        direction.Normalise();
        float lambert = Clamp01(direction.Y * normal.Y + direction.X * normal.X + direction.Z * normal.Z);
        if (lambert > g_epsilon_0129ba3c) {
            float attenuation = 1.0f / ((light.WorldPos - position).LengthSquared() * light.AttenuationDistSquared) + light.AttenuationConstant;
            colour += light.Colour * Clamp01(lambert * attenuation);
        }
    }
    if (colour.R > 1.0f) colour.R = 1.0f;
    if (colour.G > 1.0f) colour.G = 1.0f;
    if (colour.B > 1.0f) colour.B = 1.0f;
    return CRGBFloatColour(colour.R, colour.G, colour.B, 1.0f);
}
