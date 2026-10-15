#include <cstdio>
// Behaviour of CParticleSystem::ScaleParticleDimensions (0x00AF8F80): with ScaleParticles set the size is multiplied
// by the largest Scale component, otherwise it is left alone.
#define PS_MAX(a, b) ((a) > (b) ? (a) : (b))
struct C3DVector { float X, Y, Z; };
struct CParticleSystem {
    unsigned char m_pad00[0x50];
    bool Initialised : 1; bool Enabled : 1; bool Dead : 1; bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1; bool Visible : 1; bool StayWithEmitter : 1; bool ScaleParticles : 1;
    float GlobalAlpha; float FadeAlpha; float NormalisedTime;
    C3DVector Scale;
    void ScaleParticleDimensions(float* size)
    {
        if (ScaleParticles)
            *size *= PS_MAX(PS_MAX(Scale.X, Scale.Y), Scale.Z);
    }
};
int main()
{
    CParticleSystem s; s.ScaleParticles = true; s.StayWithEmitter = false;
    float v;
    s.Scale.X = 2.0f; s.Scale.Y = 1.0f; s.Scale.Z = 0.5f; v = 3.0f; s.ScaleParticleDimensions(&v); if (v != 6.0f) { std::printf("00af8f80_TEST FAIL\n"); return 1; }
    s.Scale.X = 1.0f; s.Scale.Y = 4.0f; s.Scale.Z = 0.5f; v = 3.0f; s.ScaleParticleDimensions(&v); if (v != 12.0f) { std::printf("00af8f80_TEST FAIL\n"); return 1; }
    s.Scale.X = 1.0f; s.Scale.Y = 2.0f; s.Scale.Z = 5.0f; v = 3.0f; s.ScaleParticleDimensions(&v); if (v != 15.0f) { std::printf("00af8f80_TEST FAIL\n"); return 1; }
    s.Scale.X = 3.0f; s.Scale.Y = 1.0f; s.Scale.Z = 5.0f; v = 2.0f; s.ScaleParticleDimensions(&v); if (v != 10.0f) { std::printf("00af8f80_TEST FAIL\n"); return 1; }
    s.ScaleParticles = false; v = 2.0f; s.ScaleParticleDimensions(&v); if (v != 2.0f) { std::printf("00af8f80_TEST FAIL\n"); return 1; }
    std::printf("00af8f80_TEST PASS\n");
    return 0;
}
