#include <stdio.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
class CPlane { public: C3DVector Normal; float Distance; };
class CEngineCamera { public:
    C3DVector WorldPosition; float NearZPlane; float FarZPlane; C3DVector LookVector; CPlane FrustrumPlanes[4];
};
class CEngineSimulationManager { public: float SimRadius; float Zoom; };
static CEngineCamera* g_pEngineCamera;
static CEngineSimulationManager* g_pEngineSimulationManager;
static int CalculateRunState(const C3DVector& position, float radius)
{
    const CEngineCamera* camera = g_pEngineCamera;
    C3DVector toCamera = camera->WorldPosition - position;
    float simRadius = g_pEngineSimulationManager->SimRadius * g_pEngineSimulationManager->Zoom;
    if (!(toCamera.GetMagnitudeSquared() < simRadius * simRadius))
        return 1;
    C3DVector offset = position - camera->WorldPosition;
    float depth = offset * camera->LookVector;
    if (depth + radius < camera->NearZPlane || depth - radius > camera->FarZPlane
        || offset * camera->FrustrumPlanes[2].Normal > radius || offset * camera->FrustrumPlanes[3].Normal > radius
        || offset * camera->FrustrumPlanes[0].Normal > radius || offset * camera->FrustrumPlanes[1].Normal > radius)
        return 1;
    return 0;
}
int main()
{
    static CEngineCamera cam;
    cam.WorldPosition = C3DVector(0, 0, 0); cam.NearZPlane = 1.0f; cam.FarZPlane = 100.0f; cam.LookVector = C3DVector(0, 1, 0);
    // side planes facing outwards: x = +-y, z = +-y
    cam.FrustrumPlanes[0].Normal = C3DVector(1, -1, 0);  cam.FrustrumPlanes[1].Normal = C3DVector(-1, -1, 0);
    cam.FrustrumPlanes[2].Normal = C3DVector(0, -1, 1);  cam.FrustrumPlanes[3].Normal = C3DVector(0, -1, -1);
    static CEngineSimulationManager sim; sim.SimRadius = 25.0f; sim.Zoom = 2.0f;
    g_pEngineCamera = &cam; g_pEngineSimulationManager = &sim;
    int ok = CalculateRunState(C3DVector(0, 10, 0), 1.0f) == 0
        && CalculateRunState(C3DVector(0, 60, 0), 1.0f) == 1          // beyond the 50-unit simulation radius
        && CalculateRunState(C3DVector(0, -5, 0), 1.0f) == 1          // behind the near plane
        && CalculateRunState(C3DVector(30, 10, 0), 1.0f) == 1         // outside a side plane
        && CalculateRunState(C3DVector(0, 0.5f, 0), 1.0f) == 0;       // near plane overlap counts as visible
    if (!ok) { printf("00b80940_TEST FAIL\n"); return 1; }
    printf("00b80940_TEST PASS\n");
    return 0;
}