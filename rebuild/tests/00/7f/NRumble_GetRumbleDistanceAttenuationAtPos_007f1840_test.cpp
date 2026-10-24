#include <cstdio>
#include <cmath>
// Behaviour test: NRumble::GetRumbleDistanceAttenuationAtPos -- exp(-0.01 * squared distance).
#define CHECK(c) if (!(c)) { std::printf("007f1840_TEST FAIL\n"); return 1; }
struct C3DVector { float X, Y, Z; };
static float Attenuation(const C3DVector& pos, const C3DVector& source)
{
    float dx = pos.X - source.X, dy = pos.Y - source.Y, dz = pos.Z - source.Z;
    float distSq = dx * dx + dy * dy + dz * dz;
    return (float)std::exp(distSq * -0.01f);
}
static bool Near(float a, float b) { return a - b < 1e-5f && b - a < 1e-5f; }
int main()
{
    C3DVector a = { 1.0f, 2.0f, 3.0f }, b = { 1.0f, 2.0f, 3.0f }, c = { 4.0f, 6.0f, 3.0f };
    CHECK(Near(Attenuation(a, b), 1.0f))
    CHECK(Near(Attenuation(a, c), (float)std::exp(-0.25)))
    CHECK(Near(Attenuation(a, c), Attenuation(c, a)))
    std::printf("007f1840_TEST PASS\n");
    return 0;
}