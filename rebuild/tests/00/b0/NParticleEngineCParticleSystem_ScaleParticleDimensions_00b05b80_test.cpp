// behaviour test for 0x00B05B80 CParticleSystem::ScaleParticleDimensions(long&): scale by the truncated largest Scale component
#include <cstdio>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
class C3DVector { public: float X, Y, Z; };
struct CParticleSystem {
    bool ScaleParticles; C3DVector Scale;
    void ScaleParticleDimensions(int& d) const { if (ScaleParticles) d *= (int)MAX(MAX(Scale.X, Scale.Y), Scale.Z); }
};
int main()
{
    CParticleSystem s; s.ScaleParticles = true; s.Scale.X = 1.5f; s.Scale.Y = 2.9f; s.Scale.Z = 0.5f;
    int a = 10; s.ScaleParticleDimensions(a);
    s.ScaleParticles = false; int b = 10; s.ScaleParticleDimensions(b);
    if (a != 20 || b != 10) { std::printf("00b05b80_TEST FAIL\n"); return 1; }
    std::printf("00b05b80_TEST PASS\n");
    return 0;
}