#include <cstdio>
// Behaviour of CParticleSystem::GetParticleWorldTransform (0x00B05980): the unscaled transform with the diagonal scaled by Scale when ScaleParticles is set.
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float m[12]; };
static CMatrix3x4 g_emitterTransform;
struct CParticleSystem {
    unsigned char m_pad00[0x50];
    bool Initialised : 1; bool Enabled : 1; bool Dead : 1; bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1; bool Visible : 1; bool StayWithEmitter : 1; bool ScaleParticles : 1;
    float GlobalAlpha; float FadeAlpha; float NormalisedTime;
    C3DVector Scale;
    const CMatrix3x4* GetSystemTransform() const { return &g_emitterTransform; }
    void GetUnscaledWorldTransform(CMatrix3x4* result)
    {
        if (StayWithEmitter) {
            *result = *GetSystemTransform();
        } else {
            result->m[1] = result->m[2] = result->m[3] = result->m[5] = result->m[6] = result->m[7] =
                result->m[9] = result->m[10] = result->m[11] = 0.0f;
            result->m[0] = result->m[4] = result->m[8] = 1.0f;
        }
    }
    void GetParticleWorldTransform(CMatrix3x4* result)
    {
        GetUnscaledWorldTransform(result);
        if (ScaleParticles) {
            result->m[0] *= Scale.X;
            result->m[4] *= Scale.Y;
            result->m[8] *= Scale.Z;
        }
    }
};
int main()
{
    CParticleSystem s; s.StayWithEmitter = false; s.ScaleParticles = true;
    s.Scale.X = 2.0f; s.Scale.Y = 3.0f; s.Scale.Z = 4.0f;
    CMatrix3x4 r;
    s.GetParticleWorldTransform(&r);
    if (r.m[0] != 2.0f || r.m[4] != 3.0f || r.m[8] != 4.0f || r.m[1] != 0.0f || r.m[11] != 0.0f) { std::printf("00b05980_TEST FAIL\n"); return 1; }
    s.ScaleParticles = false;
    s.GetParticleWorldTransform(&r);
    if (r.m[0] != 1.0f || r.m[4] != 1.0f || r.m[8] != 1.0f) { std::printf("00b05980_TEST FAIL\n"); return 1; }
    std::printf("00b05980_TEST PASS\n");
    return 0;
}