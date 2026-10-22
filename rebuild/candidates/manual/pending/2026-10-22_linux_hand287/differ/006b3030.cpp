// Grade: DIFFER(619v619) -- only the epilogue's pop edi: VC7.1 (RTM and QFE) hoists it to just after the last [edi] read; retail keeps it at the end
// CEnvironment::UpdateWind() -- unless the wind direction is overridden: turn the wind direction by a random angle and renormalise it, jitter the wind speed (clamped to 0.04..0.2), then ease the theme wind direction (renormalised) and speed towards them by a factor that grows with the speed
// Leaf: UpdateWind
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B3030, length 0x26B bytes (+ int3 padding), plain `ret`.
// Callees: GFFloatRandom(float range, unsigned long* seed) 0x004978A0 (__fastcall, the seed 0x013BA8DC in ecx), C3DVector two-angle
//   Rotate 0x00A14260. Globals: the reciprocal-square-root mantissa table 0x013CE558. Float pool: 97.5f 0x0125D4F4, 1/360
//   0x01238E00, 0.225f 0x0125D4F0, 0.04f 0x0125B25C, 0.2f 0x012305A0, 6.25f 0x0125D108, 0.09f 0x0125D4EC, 0.01f 0x01230A5C,
//   3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: CEnvironmentGameTickState fields 0x94 below Ego_r.pdb (landed CEnvironment::OverrideWindDirection 0x006B2B10):
//   WindSpeed +0xc10, WindDirection +0xc14, ThemeWindSpeed +0xc20, ThemeWindDirection +0xc24, OverrideWindDirection +0xc60.
// Shape: C3DVector's inline fast Normalise (landed CTCAssassinRush::SetupFacingDirection 0x007A6E20 form) twice.
extern unsigned long g_InvSqrtMantissaTable[128];  // 0x013CE558
extern unsigned long g_WindRandomSeed;             // 0x013BA8DC
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

inline void ClampInPlace(float& value, float lo, float hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}

#pragma pack(push, 1)
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    void Rotate(float zAngle, float xAngle);      // 0x00A14260
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z + y * y + x * x;
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

struct CEnvironment {
    unsigned char _pad0[0xc10];
    float         WindSpeed;                 // +0xc10
    C3DVector     WindDirection;             // +0xc14
    float         ThemeWindSpeed;            // +0xc20
    C3DVector     ThemeWindDirection;        // +0xc24
    unsigned char _pad30[0xc60 - 0xc30];
    bool          OverrideWindDirectionFlag; // +0xc60 (PDB: OverrideWindDirection)
    void UpdateWind();
};
#pragma pack(pop)

void CEnvironment::UpdateWind()
{
    if (OverrideWindDirectionFlag)
        return;
    float angle = (GFFloatRandom(195.0f, &g_WindRandomSeed) - 97.5f) * 0.0027777778f;
    WindDirection.Rotate(angle, 0.0f);
    WindDirection.Normalise();
    WindSpeed = WindSpeed - 0.225f + GFFloatRandom(0.45f, &g_WindRandomSeed);
    ClampInPlace(WindSpeed, 0.04f, 0.2f);
    float fraction = (WindSpeed - 0.04f) * 6.25f;
    float blend = fraction * 0.09f + 0.01f;
    ThemeWindDirection += (WindDirection - ThemeWindDirection) * blend;
    ThemeWindDirection.Normalise();
    ThemeWindSpeed += (WindSpeed - ThemeWindSpeed) * blend;
}
