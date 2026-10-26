// Behaviour test for 0x008FBB90 NCameraModes::CPhysicsForceModelBase::PotentialFieldBufferPoints (forward-difference gradient of a summed 1/(d - r) potential).
#include <cstdio>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
struct Node { Node* next; Node* prev; C3DVector data; };

static const C3DVector g_Offsets[4] = { C3DVector(0, 0, 0), C3DVector(1, 0, 0), C3DVector(0, 1, 0), C3DVector(0, 0, 1) };

static float PotentialField(const C3DVector& pos, const C3DVector& src, float strength, float radius)
{
    return (strength * 0.1f) / ((pos - src).GetLength() - radius);
}
static C3DVector PotentialFieldBufferPoints(const C3DVector& pos, Node* sentinel, float strength, float radius)
{
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (Node* n = sentinel->next; n != sentinel; n = n->next)
        for (int i = 0; i < 4; ++i)
            potential[i] += PotentialField(pos + g_Offsets[i] * 0.1f, n->data, strength, radius);
    return C3DVector((potential[1] - potential[0]) / 0.1f, (potential[2] - potential[0]) / 0.1f,
                     (potential[3] - potential[0]) / 0.1f);
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f * (1.0f + std::fabs(b)); }

int main()
{
    Node sentinel; sentinel.next = &sentinel; sentinel.prev = &sentinel;
    C3DVector pos(0, 0, 0);
    C3DVector g = PotentialFieldBufferPoints(pos, &sentinel, 5.0f, 1.0f);
    if (g.X != 0 || g.Y != 0 || g.Z != 0) { std::printf("008fbb90_TEST FAIL\n"); return 1; }   // no points: zero
    Node a; a.data = C3DVector(10, 0, 0);
    sentinel.next = &a; a.next = &sentinel; a.prev = &sentinel; sentinel.prev = &a;
    g = PotentialFieldBufferPoints(pos, &sentinel, 5.0f, 1.0f);
    // potential 0.5 / (d - 1): at the origin d = 10, one step towards the point d = 9.9
    float want = (0.5f / 8.9f - 0.5f / 9.0f) / 0.1f;
    float side = (0.5f / ((float)std::sqrt(100.0f + 0.01f) - 1.0f) - 0.5f / 9.0f) / 0.1f;
    if (!Near(g.X, want) || !Near(g.Y, side) || !Near(g.Z, side) || !(g.X > 0)) { std::printf("008fbb90_TEST FAIL\n"); return 1; }
    std::printf("008fbb90_TEST PASS\n");
    return 0;
}