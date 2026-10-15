// Behaviour test for 0x00b52090 CEngineWeatherRenderer::IsPointInPrecipitation
#include <cstdio>
#include <cmath>
struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    float Length() const { return (float)std::sqrt(x * x + y * y + z * z); }
};
struct C3DBoundingSphere { C3DVector Centre; float Radius; C3DVector GetCentre() const { return Centre; } };
struct CWeatherMask { C3DBoundingSphere BSphere; };
struct CEngineWeatherRenderer {
    CWeatherMask WeatherMaskList[3]; float m_boxRadiusSquared;
    float GetBoxBoundingSphereRadiusSquared() { return m_boxRadiusSquared; }
    long IsPointInPrecipitation(const C3DVector& point)
    {
        float boxRadius = (float)std::sqrt(GetBoxBoundingSphereRadiusSquared());
        for (long i = 0; i < 3; ++i) {
            const C3DBoundingSphere& sphere = WeatherMaskList[i].BSphere;
            if ((point - sphere.GetCentre()).Length() < boxRadius + sphere.Radius) return 1;
        }
        return 0;
    }
};
int main()
{
    CWeatherMask m0 = { { C3DVector(0, 0, 0), 1.0f } }, m1 = { { C3DVector(100, 0, 0), 5.0f } }, m2 = { { C3DVector(0, 50, 0), 2.0f } };
    CEngineWeatherRenderer r = { { m0, m1, m2 }, 4.0f };   // box radius 2
    bool ok = r.IsPointInPrecipitation(C3DVector(2.5f, 0, 0)) == 1      // inside mask 0 (1 + 2)
           && r.IsPointInPrecipitation(C3DVector(106, 0, 0)) == 1       // inside mask 1 (5 + 2)
           && r.IsPointInPrecipitation(C3DVector(0, 54.5f, 0)) == 0     // outside mask 2 (2 + 2)
           && r.IsPointInPrecipitation(C3DVector(0, 0, 3.0f)) == 0;     // exactly on the edge is outside
    if (!ok) { std::printf("00b52090_TEST FAIL\n"); return 1; }
    std::printf("00b52090_TEST PASS\n");
    return 0;
}