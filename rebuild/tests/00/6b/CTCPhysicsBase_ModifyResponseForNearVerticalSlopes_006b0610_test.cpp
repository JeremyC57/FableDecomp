// Behaviour test for CTCPhysicsBase::ModifyResponseForNearVerticalSlopes (0x006B0610).
#include <cstdio>
#include <cmath>
#include <cstring>

static unsigned long g_InvSqrtMantissaTable[128];
static void InitTable()
{
    // entry i: index bit 6 = exponent LSB, bits 0..5 = top mantissa bits; holds the top 8 mantissa bits of 1/sqrt
    for (int i = 0; i < 128; ++i) {
        int e = ((i >> 6) & 1) ? 127 : 128;          // biased exponent with that LSB
        double v = std::ldexp(1.0 + ((i & 63) + 0.5) / 64.0, e - 127);
        int E = ((380 - e) >> 1) - 127;
        double mant = (1.0 / std::sqrt(v)) / std::ldexp(1.0, E) - 1.0;
        long t = (long)(mant * 256.0 + 0.5);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)t;
    }
}

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise()
    {
        float x = X, y = Y, z = Z;
        float lengthSquared = x * x + (y * y + z * z);
        unsigned long bits; std::memcpy(&bits, &lengthSquared, 4);
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r; std::memcpy(&r, &estimate, 4);
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r; Y *= r; Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
class CPhysicsCollideResponse { public: C3DVector CollidePos; C3DVector CollideNormal; float Friction; };
struct CPhysicsGlobalDef { float MaxSlopeAngle; };
struct CDefinitionHolder { CPhysicsGlobalDef* PhysicsDef; };
class CThing { public: unsigned char ThingClass; };
static CPhysicsGlobalDef g_def;
static CDefinitionHolder g_holder = { &g_def };

class CTCPhysicsBase { public:
    CThing* ParentThing;
    CDefinitionHolder* GetDefinitions() const { return &g_holder; }
    void ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse& response) const
    {
        if (ParentThing->ThingClass == 2) {
            float up = Dot(response.CollideNormal, C3DVector(0.0f, 0.0f, 1.0f));
            float angle = GetDefinitions()->PhysicsDef->MaxSlopeAngle;
            if (angle < 45.0f)
                angle = 45.0f;
            else if (angle > 89.0f)
                angle = 89.0f;
            float limit = (float)std::cos(angle / 360.0f * 6.2831854820251465);
            if (std::fabs(up) < limit) {
                response.CollideNormal.Z = 0.0f;
                response.CollideNormal.Normalise();
            }
        }
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 0.01f; }

int main()
{
    InitTable();
    CThing thing; thing.ThingClass = 2;
    CTCPhysicsBase p; p.ParentThing = &thing;
    CPhysicsCollideResponse r;
    bool ok = true;
    g_def.MaxSlopeAngle = 60.0f;                  // cos 60 = 0.5
    r.CollideNormal = C3DVector(0.6f, 0.0f, 0.3f); // steep (up 0.3 < 0.5): flattened to (1, 0, 0)
    p.ModifyResponseForNearVerticalSlopes(r);
    ok = ok && Near(r.CollideNormal.X, 1.0f) && r.CollideNormal.Z == 0.0f;
    r.CollideNormal = C3DVector(0.6f, 0.0f, 0.8f); // gentle (0.8 >= 0.5): unchanged
    p.ModifyResponseForNearVerticalSlopes(r);
    ok = ok && r.CollideNormal.X == 0.6f && r.CollideNormal.Z == 0.8f;
    g_def.MaxSlopeAngle = 10.0f;                  // clamped to 45: cos 45 = 0.707
    r.CollideNormal = C3DVector(0.0f, 0.8f, 0.6f); // 0.6 < 0.707: flattened to (0, 1, 0)
    p.ModifyResponseForNearVerticalSlopes(r);
    ok = ok && Near(r.CollideNormal.Y, 1.0f) && r.CollideNormal.Z == 0.0f;
    thing.ThingClass = 1;                         // other classes untouched
    r.CollideNormal = C3DVector(0.6f, 0.0f, 0.3f);
    p.ModifyResponseForNearVerticalSlopes(r);
    ok = ok && r.CollideNormal.Z == 0.3f;
    if (!ok) { std::printf("006b0610_TEST FAIL\n"); return 1; }
    std::printf("006b0610_TEST PASS\n");
    return 0;
}