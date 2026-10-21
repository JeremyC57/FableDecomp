// Behaviour test for NParticleEngine::CPSCLight::AddToBoundingSphere @ 0x00B18AF0.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
static float g_ParticleEpsilon = 0.0001f;
static long g_frameRate = 30;
long GetParticleFrameRate() { return g_frameRate; }
inline float Lerp(float a, float b, float t) { return (b - a) * t + a; }

struct C3DBoundingSphere {
    C3DVector point; float radius; int calls;
    void AddToBounds(const C3DVector& p, float r) { point = p; radius = r; ++calls; }
};
struct CParticleSystem {
    unsigned char Flags; float scale;
    void GetInverseSystemTransform(CMatrix3x4& m)
    {
        m.E11 = 1; m.E12 = 0; m.E13 = 0; m.E21 = 0; m.E22 = 1; m.E23 = 0; m.E31 = 0; m.E32 = 0; m.E33 = 1;
        m.E41 = 10; m.E42 = 20; m.E43 = 30;
    }
    void ScaleParticleDimensions(float& size) { size *= scale; }
};
struct CPSCLight {
    long LightLifeTimer; bool LightIsAlive; unsigned long LightPositionParam; float LightLifeSecs;
    float LightStartRenderWorldRadius, LightEndRenderWorldRadius; bool LightUseLifeSecs;
    C3DVector GetCurrentPosition(unsigned long, float) const { C3DVector v; v.X = 1; v.Y = 2; v.Z = 3; return v; }
    bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system);
};

bool CPSCLight::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system)
{
    if (LightIsAlive) {
        C3DVector pos = GetCurrentPosition(LightPositionParam, timeOffset);
        if (!(system.Flags & 0x40)) {
            CMatrix3x4 m;
            system.GetInverseSystemTransform(m);
            float x = pos.X, y = pos.Y, z = pos.Z;
            pos.X = m.E31 * z; pos.X += m.E11 * x; pos.X += m.E21 * y; pos.X += m.E41;
            pos.Y = m.E32 * z; pos.Y += m.E12 * x; pos.Y += m.E22 * y; pos.Y += m.E42;
            pos.Z = m.E33 * z; pos.Z += m.E23 * y; pos.Z += m.E13 * x; pos.Z += m.E43;
        }
        float frameRate = (float)GetParticleFrameRate();
        float t = 0.0f;
        if (LightUseLifeSecs) {
            t = ((float)LightLifeTimer + timeOffset) / (frameRate * LightLifeSecs);
            if (t < 0.0f) t = 0.0f;
            else { float limit = 1.0f - g_ParticleEpsilon; if (t > limit) t = limit; }
        }
        timeOffset = Lerp(LightStartRenderWorldRadius, LightEndRenderWorldRadius, t);
        system.ScaleParticleDimensions(timeOffset);
        sphere.AddToBounds(pos, timeOffset);
        return true;
    }
    return false;
}

int main()
{
    CPSCLight l; l.LightLifeTimer = 15; l.LightIsAlive = false; l.LightPositionParam = 0; l.LightLifeSecs = 1.0f;
    l.LightStartRenderWorldRadius = 2.0f; l.LightEndRenderWorldRadius = 4.0f; l.LightUseLifeSecs = true;
    C3DBoundingSphere s; s.calls = 0; s.radius = 0;
    CParticleSystem sys; sys.Flags = 0; sys.scale = 2.0f;
    int ok = !l.AddToBoundingSphere(s, 0.0f, sys) && s.calls == 0;
    l.LightIsAlive = true;
    ok = ok && l.AddToBoundingSphere(s, 0.0f, sys) && s.calls == 1 && s.radius == 6.0f   // t = 0.5 -> 3, scaled x2
          && s.point.X == 11 && s.point.Y == 22 && s.point.Z == 33;
    sys.Flags = 0x40; l.LightLifeTimer = 300;                                          // clamps to 1 - eps, local space
    ok = ok && l.AddToBoundingSphere(s, 0.0f, sys) && s.point.X == 1 && s.radius > 7.99f && s.radius < 8.0f;
    l.LightUseLifeSecs = false;
    ok = ok && l.AddToBoundingSphere(s, 0.0f, sys) && s.radius == 4.0f;
    if (!ok) { std::printf("00b18af0_TEST FAIL\n"); return 1; }
    std::printf("00b18af0_TEST PASS\n");
    return 0;
}
