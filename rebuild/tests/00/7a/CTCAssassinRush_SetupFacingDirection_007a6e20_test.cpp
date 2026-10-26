// Behaviour test for CTCAssassinRush::SetupFacingDirection (007a6e20): table-driven normalise, then the facing replaces a
// direction within 60 degrees of straight up.
#include <cstdio>
#include <math.h>

unsigned int g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    for (int t = 0; t < 128; ++t) {
        int eLow = t >> 6, m = t & 63;
        unsigned int bitsE = eLow ? 127 : 128;                    // input exponent whose low bit is eLow
        double x = ldexp(1.0 + (m + 0.5) / 64.0, (int)bitsE - 127);
        double r = 1.0 / sqrt(x);
        int outE = (int)((0x17c - bitsE) >> 1) - 127;
        long v = (long)floor((r / ldexp(1.0, outE) - 1.0) * 256.0 + 0.5);
        if (v < 0) v = 0; if (v > 255) v = 255;
        g_InvSqrtMantissaTable[t] = (unsigned int)v;
    }
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise()
    {
        float x = X; float y = Y; float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned int bits = *(unsigned int*)&lengthSquared;
        unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r; Y *= r; Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static int g_facingCalls = 0;
struct CPhysicsBody { virtual C3DVector GetFacing() { ++g_facingCalls; return C3DVector(0.0f, 1.0f, 0.0f); } };
class CThing { public: CPhysicsBody* PhysicsBody; };
class CTCAssassinRush {
public:
    CThing* ParentThing;
    void SetupFacingDirection(C3DVector& direction)
    {
        direction.Normalise();
        if (Dot(direction, C3DVector(0.0f, 0.0f, 1.0f)) > cos(1.0471976115464443))
            direction = ParentThing->PhysicsBody->GetFacing();
    }
};
static bool Near(float a, float b) { return fabs(a - b) < 0.01f; }

int main()
{
    if (sizeof(unsigned int) != 4) { g_InvSqrtMantissaTable[0] = 0; }   // 64-bit hosts: the bit trick needs 4-byte words
    BuildTable();
    CPhysicsBody body; CThing thing; thing.PhysicsBody = &body;
    CTCAssassinRush rush; rush.ParentThing = &thing;
    bool ok = true;
    C3DVector side(3.0f, 0.0f, 4.0f);                 // 37 degrees from up: replaced
    rush.SetupFacingDirection(side);
    ok = ok && g_facingCalls == 1 && side.X == 0.0f && side.Y == 1.0f && side.Z == 0.0f;
    C3DVector flat(0.0f, 4.0f, 1.0f);                 // 76 degrees from up: kept, normalised
    rush.SetupFacingDirection(flat);
    ok = ok && g_facingCalls == 1 && Near(flat.X, 0.0f) && Near(flat.Y, 0.9701f) && Near(flat.Z, 0.2425f);
    C3DVector up(0.0f, 0.0f, 2.0f);
    rush.SetupFacingDirection(up);
    ok = ok && g_facingCalls == 2 && up.Y == 1.0f;
    if (!ok) { std::printf("007a6e20_TEST FAIL\n"); return 1; }
    std::printf("007a6e20_TEST PASS\n");
    return 0;
}