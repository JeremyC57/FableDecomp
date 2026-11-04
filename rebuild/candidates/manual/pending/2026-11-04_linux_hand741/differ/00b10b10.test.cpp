// Behaviour test for NParticleEngine::CPSCEmitterGeneric::GenerateParticle (0x00B10B10): a standalone model of the direction / speed
// rules -- the random-2D direction projected off the up axis and renormalised, OppositeDirection negation, the angular perturbation
// (random point minus its projection on the direction, scaled by AngularPerturbationInteger * 0.001, renormalised to the original
// length), and the speed MinSpeed + random * max(MaxSpeed - MinSpeed, 0).
#include <cstdio>
#include <cmath>
struct Vec { float X, Y, Z; };
static float Dot(const Vec& a, const Vec& b) { return a.X * b.X + a.Z * b.Z + a.Y * b.Y; }
static Vec Sub(const Vec& a, const Vec& b) { Vec r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static Vec Scale(const Vec& a, float s) { Vec r = { a.X * s, a.Y * s, a.Z * s }; return r; }
static float Length(const Vec& a) { return std::sqrt(Dot(a, a)); }
static bool IsNonZero(const Vec& a) { return Dot(a, a) > 0.0001f * 0.0001f; }
static Vec Normalise(const Vec& a) { return Scale(a, 1.0f / Length(a)); }
static Vec Random2D(const Vec& random, const Vec& up)
{
    Vec direction = Sub(random, Scale(up, Dot(up, random)));
    return IsNonZero(direction) ? Normalise(direction) : direction;
}
static Vec Perturb(const Vec& direction, const Vec& random, long angularPerturbation)
{
    float length = Length(direction);
    Vec side = Sub(random, Scale(direction, Dot(random, direction)));
    Vec result = direction;
    Vec add = Scale(side, angularPerturbation * 0.001f);
    result.X += add.X; result.Y += add.Y; result.Z += add.Z;
    if (IsNonZero(result))
        result = Scale(Normalise(result), length);
    return result;
}
static float Speed(float minSpeed, float maxSpeed, float random01)
{
    float range = maxSpeed - minSpeed;
    if (!(range > 0.0f))
        range = 0.0f;
    return random01 * range + minSpeed;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Vec up = { 0.0f, 0.0f, 1.0f }, random = { 0.6f, 0.0f, 0.8f };
    Vec d = Random2D(random, up);
    bool ok = Near(d.X, 1.0f) && Near(d.Z, 0.0f);                              // projected onto the plane, unit length
    Vec opposite = Scale(d, -1.0f);
    ok = ok && Near(opposite.X, -1.0f);
    Vec dir = { 0.0f, 0.0f, 1.0f }, rnd = { 1.0f, 0.0f, 0.5f };
    Vec p = Perturb(dir, rnd, 1000);                                          // unit direction: 45 degrees off, length kept
    ok = ok && Near(Length(p), 1.0f) && Near(p.X, p.Z);
    ok = ok && Near(Speed(1.0f, 3.0f, 0.5f), 2.0f) && Near(Speed(3.0f, 1.0f, 0.5f), 3.0f);
    printf(ok ? "00b10b10_TEST PASS\n" : "00b10b10_TEST FAIL\n");
    return ok ? 0 : 1;
}
