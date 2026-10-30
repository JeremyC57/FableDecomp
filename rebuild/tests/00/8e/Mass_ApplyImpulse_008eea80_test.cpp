// Behaviour test for Mass::ApplyImpulse (0x008EEA80): angular velocity += (perp(point - Pos) . impulse) * AIMass,
// linear velocity += impulse * IMass, with perp(v) = (v.Y, -v.X).
#include <cstdio>
#include <cmath>

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float operator*(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
    C2DVector GetPerpendicular() const { return C2DVector(Y, -X); }
};

class Mass { public:
    C2DVector Pos;
    C2DVector Vel;
    float IMass;
    float APos;
    float AVel;
    float AIMass;
    void ApplyImpulse(C2DVector point, C2DVector impulse)
    {
        AVel += ((point - Pos).GetPerpendicular() * impulse) * AIMass;
        Vel += impulse * IMass;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    Mass m;
    m.Pos = C2DVector(1.0f, 2.0f);
    m.Vel = C2DVector(0.5f, -0.5f);
    m.IMass = 0.25f;
    m.APos = 0.0f;
    m.AVel = 1.0f;
    m.AIMass = 2.0f;
    m.ApplyImpulse(C2DVector(4.0f, 6.0f), C2DVector(2.0f, 1.0f));
    // r = (3, 4); perp = (4, -3); perp . impulse = 8 - 3 = 5; AVel = 1 + 5 * 2 = 11
    if (!near(m.AVel, 11.0f) || !near(m.Vel.X, 1.0f) || !near(m.Vel.Y, -0.25f) || !near(m.APos, 0.0f)) {
        std::printf("008eea80_TEST FAIL\n");
        return 1;
    }
    // an impulse through the centre of mass adds no spin
    m.ApplyImpulse(C2DVector(1.0f, 2.0f), C2DVector(3.0f, 3.0f));
    if (!near(m.AVel, 11.0f) || !near(m.Vel.X, 1.75f) || !near(m.Vel.Y, 0.5f)) {
        std::printf("008eea80_TEST FAIL\n");
        return 1;
    }
    std::printf("008eea80_TEST PASS\n");
    return 0;
}