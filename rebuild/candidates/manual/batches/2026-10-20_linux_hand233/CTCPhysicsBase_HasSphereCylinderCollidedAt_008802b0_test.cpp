// Behaviour test for CTCPhysicsBase::HasSphereCylinderCollidedAt (0x008802B0).
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

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    void Normalise()
    {
        float x = X, y = Y, z = Z;
        float lengthSquared = x * x;
        lengthSquared += y * y;
        lengthSquared += z * z;
        unsigned long bits; std::memcpy(&bits, &lengthSquared, 4);
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r; std::memcpy(&r, &estimate, 4);
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r; Y *= r; Z *= r;
    }
};
struct CPhysicsCollideResponse { C3DVector CollidePos; C3DVector CollideNormal; float Friction; };
struct CTCPhysicsBase {
    C3DVector Position;
    float     Radius;
    bool HasSphereCylinderCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const
    {
        C3DVector d = pos - other->Position;
        float radii = other->Radius + Radius;
        if (d.X * d.X + d.Y * d.Y < radii * radii && std::fabs(d.Z) < 1.6f) {
            d.Z = 0.0f;
            d.Normalise();
            response->CollideNormal = d;
            response->CollidePos = pos;
            return true;
        }
        return false;
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 0.01f; }

int main()
{
    InitTable();
    CTCPhysicsBase self, other;
    self.Position = C3DVector(0, 0, 0); self.Radius = 1.0f;
    other.Position = C3DVector(0, 0, 0); other.Radius = 2.0f;
    CPhysicsCollideResponse resp;
    std::memset(&resp, 0, sizeof(resp));
    bool ok = true;
    // inside horizontally (dist 2.4 < 3), small height difference -> hit, normal (0.6, 0.8, 0)
    ok = ok && self.HasSphereCylinderCollidedAt(&other, C3DVector(1.2f, 1.6f, 1.0f), &resp);
    ok = ok && Near(resp.CollideNormal.X, 0.6f) && Near(resp.CollideNormal.Y, 0.8f) && resp.CollideNormal.Z == 0.0f;
    ok = ok && resp.CollidePos.X == 1.2f && resp.CollidePos.Z == 1.0f;
    // too far horizontally
    ok = ok && !self.HasSphereCylinderCollidedAt(&other, C3DVector(3.0f, 0.5f, 0.0f), &resp);
    // too far vertically
    ok = ok && !self.HasSphereCylinderCollidedAt(&other, C3DVector(1.0f, 0.0f, -1.7f), &resp);
    if (!ok) { std::printf("008802b0_TEST FAIL\n"); return 1; }
    std::printf("008802b0_TEST PASS\n");
    return 0;
}
