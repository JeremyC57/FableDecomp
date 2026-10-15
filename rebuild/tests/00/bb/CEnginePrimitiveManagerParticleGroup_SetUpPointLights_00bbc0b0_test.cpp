// Behaviour test for 0x00BBC0B0 (see src/00bbc0b0.cpp): SetUpPointLights fetching lights once and returning the cached count.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00bbc0b0_TEST FAIL\n"); return 1; } } while (0)

struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { C3DVector Center; float Radius; C3DBoundingSphere(const C3DVector& c, float r) : Center(c), Radius(r) { } };
struct CEnginePrimitiveThemeChannelDesc { unsigned char a, b, c, pad; CEnginePrimitiveThemeChannelDesc() : a(0), b(0), c(0) { } };
struct CEngineLocalLightArray { signed char Count; };
struct CEngineLightingManager {
    int queries; int uploads; float radius; int maxLights; CEngineLocalLightArray lights;
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& s, int maxL, const CEnginePrimitiveThemeChannelDesc& ch)
    { ++queries; radius = s.Radius; maxLights = maxL; return (ch.a == 0 && ch.b == 0 && ch.c == 0) ? &lights : 0; }
    void SetupShaderPointLights(CEngineLocalLightArray*) { ++uploads; }
};
static CEngineLightingManager g_lighting;
struct CEnginePrimitiveManagerParticleGroup {
    bool PointLightsSet; signed char PointLightCount; C3DVector CurrentWorldPos; float CurrentBSRadius;
    int SetUpPointLights()
    {
        if (!PointLightsSet) {
            CEnginePrimitiveThemeChannelDesc channels;
            channels = CEnginePrimitiveThemeChannelDesc();
            CEngineLocalLightArray* lights = g_lighting.GetLightsInSphereFast(C3DBoundingSphere(CurrentWorldPos, CurrentBSRadius), 5, channels);
            PointLightCount = lights->Count;
            g_lighting.SetupShaderPointLights(lights);
            PointLightsSet = true;
        }
        return PointLightCount;
    }
};
int main()
{
    g_lighting.queries = g_lighting.uploads = 0; g_lighting.lights.Count = 3;
    CEnginePrimitiveManagerParticleGroup g; g.PointLightsSet = false; g.PointLightCount = 0; g.CurrentBSRadius = 7;
    g.CurrentWorldPos.X = g.CurrentWorldPos.Y = g.CurrentWorldPos.Z = 0;
    CHECK(g.SetUpPointLights() == 3 && g.PointLightsSet && g_lighting.queries == 1 && g_lighting.uploads == 1);
    CHECK(g_lighting.radius == 7 && g_lighting.maxLights == 5);
    g_lighting.lights.Count = 1;
    CHECK(g.SetUpPointLights() == 3 && g_lighting.queries == 1);
    g.PointLightsSet = false; g_lighting.lights.Count = -1;
    CHECK(g.SetUpPointLights() == -1 && g_lighting.queries == 2);
    std::printf("00bbc0b0_TEST PASS\n");
    return 0;
}