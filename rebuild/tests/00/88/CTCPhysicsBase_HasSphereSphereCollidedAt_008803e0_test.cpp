// Behaviour test for 008803e0: src/008803e0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const -- sphere/sphere test of this body placed at pos against other; on contact fill the response with the unit normal and the contact position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008803E0, length 301 bytes (+ int3 padding), `ret 0Ch`.
// Callees: none (C3DVector's inline fast Normalise: exponent (380 - e) / 2 with the 128-entry mantissa table 0x013CE558,
// one Newton step with 3.0f 0x0122DED4 and 0.5f 0x0122F59C).
// Shape: d is built component-wise with the radii folded into Z ((radius + pos.Z) - (otherRadius + other.Z)); the contact
// test is the single expression d.X*d.X + d.Y*d.Y + d.Z*d.Z against (radius + otherRadius) squared -- spelled out, not
// through an inline LengthSquared(), which keeps the reciprocal alive past the last Normalise product (`fld [d.Z];
// fmul st(1)` + a late `fstp st(0)` instead of retail's in-place `fmul [d.Z]`); Normalise copies X/Y/Z into locals and
// sums the squares as statements (reusing the stored products), then the normal is copied before the contact position.
// Layout: Ego_r.pdb CTCPhysicsBase (Position +0x0c, Radius +0x34) and CPhysicsCollideResponse (CollidePos +0x00,
// CollideNormal +0x0c, Friction +0x18); engine/CTCPhysicsBase.h flattens the vectors into floats, so the class is modelled
// locally with C3DVector members (as in the landed sibling 0x008802B0).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x;
        lengthSquared += y * y;
        lengthSquared += z * z;
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

struct CPhysicsCollideResponse {
    C3DVector CollidePos;       // +0x00
    C3DVector CollideNormal;    // +0x0c
    float     Friction;         // +0x18
};

struct CTCPhysicsBase {
    void*     __vftable;                       // +0x00
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector Velocity;                        // +0x28
    float     Radius;                          // +0x34
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const;
};

bool CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase* other, const C3DVector& pos, CPhysicsCollideResponse* response) const
{
    float otherRadius = other->Radius;
    float radius = Radius;
    C3DVector d(pos.X - other->Position.X, pos.Y - other->Position.Y, (radius + pos.Z) - (otherRadius + other->Position.Z));
    float radii = radius + otherRadius;
    if (d.X * d.X + d.Y * d.Y + d.Z * d.Z < radii * radii) {
        d.Normalise();
        response->CollideNormal = d;
        response->CollidePos = pos;
        return true;
    }
    return false;
}

// ---- stubs and driver ----
#include <math.h>
unsigned long g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        unsigned long e = 126 + ((i >> 6) & 1);
        unsigned long bits = (e << 23) | ((unsigned long)(i & 63) << 17) | (1UL << 16);
        float x = *(float*)&bits;
        float y = (float)(1.0 / sqrt((double)x));
        unsigned long ybits = *(unsigned long*)&y;
        unsigned long E = (0x17c - e) >> 1;
        unsigned long ye = (ybits >> 23) & 0xff;
        g_InvSqrtMantissaTable[i] = ye == E ? ((ybits & 0x7fffff) >> 15) : (ye > E ? 0xff : 0);
    }
}
static bool Near(float a, float b) { return fabs(a - b) < 2e-3f; }
static void Init(CTCPhysicsBase& b, float x, float y, float z, float radius)
{
    b.__vftable = 0; b.m_tcBase[0] = b.m_tcBase[1] = 0;
    b.Position = C3DVector(x, y, z); b.OldPosition = C3DVector(0, 0, 0); b.OldPositionLastFrameSet = 0;
    b.Velocity = C3DVector(0, 0, 0); b.Radius = radius;
}
int main()
{
    BuildTable();
    CTCPhysicsBase self, other;
    Init(self, 9, 9, 9, 1.0f); Init(other, 0, 0, 0, 1.0f);
    CPhysicsCollideResponse resp; resp.CollidePos = C3DVector(-1, -1, -1); resp.CollideNormal = C3DVector(-1, -1, -1); resp.Friction = 7.0f;
    // 1) touching along X: unit normal (1,0,0), contact position = pos
    C3DVector pos(1.0f, 0.0f, 0.0f);
    CHECK(self.HasSphereSphereCollidedAt(&other, pos, &resp) == true);
    CHECK(Near(resp.CollideNormal.X, 1.0f) && Near(resp.CollideNormal.Y, 0.0f) && Near(resp.CollideNormal.Z, 0.0f));
    CHECK(resp.CollidePos.X == 1.0f && resp.CollidePos.Y == 0.0f && resp.CollidePos.Z == 0.0f && resp.Friction == 7.0f);
    // 2) too far apart: false, response untouched
    resp.CollidePos = C3DVector(-1, -1, -1); resp.CollideNormal = C3DVector(-2, -2, -2);
    C3DVector far(3.0f, 0.0f, 0.0f);
    CHECK(self.HasSphereSphereCollidedAt(&other, far, &resp) == false);
    CHECK(resp.CollidePos.X == -1.0f && resp.CollideNormal.X == -2.0f && resp.CollideNormal.Z == -2.0f);
    // 3) radii folded into Z: d = (0.6, 0.8, (0.5 + 2) - (1.5 + 0)) = (0.6, 0.8, 1), |d|^2 = 2 < (0.5 + 1.5)^2
    Init(self, 0, 0, 0, 0.5f); Init(other, 0, 0, 0, 1.5f);
    C3DVector p3(0.6f, 0.8f, 2.0f);
    CHECK(self.HasSphereSphereCollidedAt(&other, p3, &resp) == true);
    float inv = 1.0f / (float)sqrt(2.0);
    CHECK(Near(resp.CollideNormal.X, 0.6f * inv) && Near(resp.CollideNormal.Y, 0.8f * inv) && Near(resp.CollideNormal.Z, inv));
    CHECK(resp.CollidePos.X == 0.6f && resp.CollidePos.Y == 0.8f && resp.CollidePos.Z == 2.0f);
    // 4) the other body's position is subtracted: other at (10,0,0), pos (11.5,0,0), radii 2 -> d = (1.5,0,0)
    Init(self, 0, 0, 0, 1.0f); Init(other, 10.0f, 0, 0, 1.0f);
    C3DVector p4(11.5f, 0.0f, 0.0f);
    CHECK(self.HasSphereSphereCollidedAt(&other, p4, &resp) == true && Near(resp.CollideNormal.X, 1.0f) && resp.CollidePos.X == 11.5f);
    // 5) exactly at the radii sum is not a collision (strict <)
    C3DVector p5(12.0f, 0.0f, 0.0f);
    CHECK(self.HasSphereSphereCollidedAt(&other, p5, &resp) == false);
    if (fails == 0) printf("008803e0_TEST PASS\n");
    return fails;
}
