// Behaviour test for 0x006AFA40 CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder: a velocity heading into the
// collision normal loses its normal component (true); one moving away or along the surface is untouched (false).
#include <stdio.h>
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLength() const
    {
        float z = Z;
        float y = Y;
        float x = X;
        return (float)sqrt(x * x + y * y + z * z);
    }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator-=(const C3DVector& o)
    {
        X -= o.X;
        Y -= o.Y;
        Z -= o.Z;
        return *this;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { float d = a.Z * b.Z; d += a.Y * b.Y; d += a.X * b.X; return d; }

class CPhysicsCollideResponse {
public:
    C3DVector CollidePos;                                   // +0x00
    C3DVector CollideNormal;                                // +0x0c
    float     Friction;                                     // +0x18
};

class CTCPhysicsBase {
public:
    bool ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& response, C3DVector& velocity) const;
};

bool CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& response, C3DVector& velocity) const
{
    float speed = velocity.GetLength();
    float into = Dot(velocity, response.CollideNormal) / speed;
    if (into < 0.0f) {
        velocity -= response.CollideNormal * into * speed;
        return true;
    }
    return false;
}

static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }

int main()
{
    int ok = 1;
    CTCPhysicsBase tc;
    CPhysicsCollideResponse r;
    r.CollidePos = C3DVector(0.0f, 0.0f, 0.0f);
    r.CollideNormal = C3DVector(1.0f, 0.0f, 0.0f);
    r.Friction = 0.0f;
    C3DVector v(-3.0f, 4.0f, 2.0f);                    // into the normal
    if (!tc.ImpulseRespondToCollisonWithCylinder(r, v)) ok = 0;
    if (!Near(v.X, 0.0f) || !Near(v.Y, 4.0f) || !Near(v.Z, 2.0f)) ok = 0;
    C3DVector w(3.0f, 4.0f, 0.0f);                      // away from it
    if (tc.ImpulseRespondToCollisonWithCylinder(r, w)) ok = 0;
    if (w.X != 3.0f || w.Y != 4.0f || w.Z != 0.0f) ok = 0;
    r.CollideNormal = C3DVector(0.0f, 0.6f, 0.8f);
    C3DVector u(1.0f, -0.6f, -0.8f);                   // dot = -1
    if (!tc.ImpulseRespondToCollisonWithCylinder(r, u)) ok = 0;
    if (!Near(u.X, 1.0f) || !Near(u.Y, 0.0f) || !Near(u.Z, 0.0f)) ok = 0;
    printf("006afa40_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}