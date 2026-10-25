// Behaviour test for 0x0080DA50 CApplyAdditionalExplosiveImpulseIfInsideSmearedLine::operator()(CThing*) const
#include <cstdio>
#include <cmath>

struct Vec {
    float X, Y, Z;
    Vec() : X(0), Y(0), Z(0) { }
    Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    Vec operator*(float s) const { return Vec(X * s, Y * s, Z * s); }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    float GetXYLength() const { return (float)std::sqrt(Y * Y + X * X); }
};
struct Physics {
    Vec Position;
    Vec LastImpulse;
    int Impulses;
    bool Moving;
    void AddAdditionalImpulse(const Vec& v) { LastImpulse = v; ++Impulses; }
    void SetAsMoving(bool m) { Moving = m; }
};
struct Thing { Physics* PhysicsTC; };

// point minus its closest point on the segment [a, b] (the engine helper 0x008F5B50)
static Vec GetLineOut(const Vec& a, const Vec& b, const Vec& p)
{
    Vec ab(b.X - a.X, b.Y - a.Y, b.Z - a.Z);
    float len2 = ab.X * ab.X + ab.Y * ab.Y + ab.Z * ab.Z;
    float t = len2 > 0 ? ((p.X - a.X) * ab.X + (p.Y - a.Y) * ab.Y + (p.Z - a.Z) * ab.Z) / len2 : 0.0f;
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return Vec(p.X - (a.X + ab.X * t), p.Y - (a.Y + ab.Y * t), p.Z - (a.Z + ab.Z * t));
}

struct SmearedLine {
    Vec StartPosition, EndPosition;
    float Radius;
    void operator()(Thing* thing) const
    {
        Physics* physics = thing->PhysicsTC;
        Vec direction = GetLineOut(StartPosition, EndPosition, physics->Position);
        direction.Z = 0.0f;
        float distance = direction.GetXYLength();
        if (distance < 0.0001f)
            direction = Vec(1.0f, 0.0f, 0.0f);
        else
            direction *= 1.0f / distance;
        if (distance < Radius) {
            physics->AddAdditionalImpulse(direction * (Radius - distance) * 0.25f);
            physics->SetAsMoving(true);
        }
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    SmearedLine f;
    f.StartPosition = Vec(0, 0, 0);
    f.EndPosition = Vec(10, 0, 0);
    f.Radius = 2.0f;
    Physics ph; Thing t; t.PhysicsTC = &ph;
    int ok = 1;
    // 1 unit beside the line (and 5 up: Z is ignored): pushed +Y by (2 - 1) * 0.25
    ph.Position = Vec(4, 1, 5); ph.Impulses = 0; ph.Moving = false;
    f(&t);
    ok &= ph.Impulses == 1 && ph.Moving && near(ph.LastImpulse.X, 0) && near(ph.LastImpulse.Y, 0.25f) && near(ph.LastImpulse.Z, 0);
    // beyond the end cap, diagonal: pushed away from the end point
    ph.Position = Vec(11, 1, 0); ph.Impulses = 0; ph.Moving = false;
    f(&t);
    float d = (float)std::sqrt(2.0f);
    ok &= ph.Impulses == 1 && near(ph.LastImpulse.X, (1 / d) * (2 - d) * 0.25f) && near(ph.LastImpulse.Y, (1 / d) * (2 - d) * 0.25f);
    // on the line: +X fallback at full strength
    ph.Position = Vec(3, 0, -7); ph.Impulses = 0; ph.Moving = false;
    f(&t);
    ok &= ph.Impulses == 1 && near(ph.LastImpulse.X, 0.5f) && near(ph.LastImpulse.Y, 0) && near(ph.LastImpulse.Z, 0);
    // outside the radius: untouched
    ph.Position = Vec(4, -3, 0); ph.Impulses = 0; ph.Moving = false;
    f(&t);
    ok &= ph.Impulses == 0 && !ph.Moving;
    std::printf(ok ? "0080da50_TEST PASS\n" : "0080da50_TEST FAIL\n");
    return ok ? 0 : 1;
}