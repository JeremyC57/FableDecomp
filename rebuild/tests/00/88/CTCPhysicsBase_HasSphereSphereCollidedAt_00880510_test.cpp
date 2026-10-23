// Behaviour test for CTCPhysicsBase::HasSphereSphereCollidedAt(other, from, to, response) (0x00880510): the model and
// body verbatim from final.cpp, the three-argument sphere test (0x008803E0) stubbed so its arguments and verdict are
// observable, the reciprocal-square-root table filled from the real formula. Checks the no-contact path (false, the
// response untouched) and the contact path: the contact point is `to` moved back along the unit from->to direction
// by both radii, then dropped by this sphere's radius.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558 (filled in main)

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
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
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos, CPhysicsCollideResponse& response) const;   // 0x008803E0
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const
{
    if (HasSphereSphereCollidedAt(other, to, response)) {
        C3DVector dir = to - from;
        dir.Normalise();
        dir *= other.Radius + Radius;
        response.CollidePos = to - dir;
        response.CollidePos.Z -= Radius;
        return true;
    }
    return false;
}

// ---- stub ----
static bool g_contact = false;
static int g_calls = 0;
static const CTCPhysicsBase* g_other = 0;
static const C3DVector* g_pos = 0;
static CPhysicsCollideResponse* g_response = 0;
bool CTCPhysicsBase::HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos, CPhysicsCollideResponse& response) const
{
    g_calls++; g_other = &other; g_pos = &pos; g_response = &response;
    return g_contact;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static bool Near(float a, float b, float eps) { float d = a - b; if (d < 0) d = -d; return d <= eps; }

int main()
{
    for (int i = 0; i < 128; ++i) {
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double x = m * ((i >> 6) ? 1.0 : 2.0);     // bit 6 set = odd exponent (x in [1,2)), clear = even (x in [2,4))
        double r = 1.0 / sqrt(x);
        long entry = (long)((2.0 * r - 1.0) * 256.0);
        if (entry < 0) entry = 0;
        if (entry > 255) entry = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)entry;
    }
    CTCPhysicsBase self; CTCPhysicsBase other;
    self.Radius = 0.5f; other.Radius = 1.5f;
    C3DVector from(1.0f, 2.0f, 3.0f);
    C3DVector to(4.0f, 6.0f, 3.0f);      // from->to = (3,4,0), length 5
    CPhysicsCollideResponse response;
    response.CollidePos = C3DVector(9.0f, 9.0f, 9.0f);
    response.CollideNormal = C3DVector(1.0f, 0.0f, 0.0f);
    response.Friction = 0.25f;

    g_contact = false;
    CHECK(self.HasSphereSphereCollidedAt(other, from, to, response) == false);
    CHECK(g_calls == 1 && g_other == &other && g_pos == &to && g_response == &response);
    CHECK(response.CollidePos.X == 9.0f && response.CollidePos.Y == 9.0f && response.CollidePos.Z == 9.0f);

    g_contact = true;
    CHECK(self.HasSphereSphereCollidedAt(other, from, to, response) == true);
    CHECK(g_calls == 2);
    // unit direction (0.6, 0.8, 0) scaled by 2.0 -> (1.2, 1.6, 0); to - that = (2.8, 4.4, 3); Z - 0.5 = 2.5
    CHECK(Near(response.CollidePos.X, 2.8f, 0.02f));
    CHECK(Near(response.CollidePos.Y, 4.4f, 0.02f));
    CHECK(Near(response.CollidePos.Z, 2.5f, 0.001f));
    CHECK(response.CollideNormal.X == 1.0f && response.Friction == 0.25f);

    printf("00880510_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}