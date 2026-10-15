// behaviour test for 0x00AF8F00 CParticleSystem::ScaleParticleDimensions(C2DVector&): scale by the largest Scale component when enabled
#include <cstdio>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; } };
struct CParticleSystem {
    bool ScaleParticles; C3DVector Scale;
    void ScaleParticleDimensions(C2DVector& d) const { if (ScaleParticles) d *= MAX(MAX(Scale.X, Scale.Y), Scale.Z); }
};
int main()
{
    CParticleSystem s; s.ScaleParticles = true; s.Scale.X = 1.0f; s.Scale.Y = 3.0f; s.Scale.Z = 2.0f;
    C2DVector a = { 2.0f, 4.0f }; s.ScaleParticleDimensions(a);
    s.Scale.Z = 5.0f; C2DVector b = { 1.0f, 1.0f }; s.ScaleParticleDimensions(b);
    s.Scale.X = 7.0f; C2DVector e = { 1.0f, 2.0f }; s.ScaleParticleDimensions(e);
    s.ScaleParticles = false; C2DVector c = { 2.0f, 2.0f }; s.ScaleParticleDimensions(c);
    if (a.X != 6.0f || a.Y != 12.0f || b.X != 5.0f || e.Y != 14.0f || c.X != 2.0f) { std::printf("00af8f00_TEST FAIL\n"); return 1; }
    std::printf("00af8f00_TEST PASS\n");
    return 0;
}