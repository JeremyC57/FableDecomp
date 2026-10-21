// behaviour test for 00b38ed0 CEnginePrimitiveAttachmentManager::IsVisible
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
struct CPlane { C3DVector Normal; float D; };
struct CCamera { C3DVector WorldPosition; float NearZPlane, FarZPlane; C3DVector LookVector; CPlane FrustrumPlanes[4]; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
static bool IsSphereInFrustum(const CCamera& camera, const C3DVector& centre, float radius)
{
    C3DVector d = centre - camera.WorldPosition;
    float z = d.Dot(camera.LookVector);
    if (z + radius < camera.NearZPlane || z - radius > camera.FarZPlane) return false;
    if (d.Dot(camera.FrustrumPlanes[2].Normal) > radius) return false;
    if (d.Dot(camera.FrustrumPlanes[3].Normal) > radius) return false;
    if (d.Dot(camera.FrustrumPlanes[0].Normal) > radius) return false;
    if (d.Dot(camera.FrustrumPlanes[1].Normal) > radius) return false;
    return true;
}
static bool IsVisible(const C3DBoundingSphere& sphere, float maxDistance, CCamera* const* first, CCamera* const* last)
{
    for (CCamera* const* it = first; it != last; ++it) {
        const CCamera& camera = **it;
        if ((camera.WorldPosition - sphere.Center).GetLength() < maxDistance - sphere.Radius) {
            if (IsSphereInFrustum(camera, sphere.Center, sphere.Radius))
                return true;
        }
    }
    return false;
}
static void Setup(CCamera& cam, float x)
{
    float k = 0.70710678f;
    cam.WorldPosition = C3DVector(x, 0, 0); cam.NearZPlane = 1; cam.FarZPlane = 100; cam.LookVector = C3DVector(0, 1, 0);
    cam.FrustrumPlanes[0].Normal = C3DVector(-k, -k, 0); cam.FrustrumPlanes[1].Normal = C3DVector(k, -k, 0);
    cam.FrustrumPlanes[2].Normal = C3DVector(0, -k, -k); cam.FrustrumPlanes[3].Normal = C3DVector(0, -k, k);
}
int main()
{
    CCamera a, b; Setup(a, 0); Setup(b, 100);
    CCamera* cams[2] = { &a, &b };
    C3DBoundingSphere s; s.Radius = 1;
    int bad = 0;
    s.Center = C3DVector(0, 10, 0);
    if (!IsVisible(s, 50, cams, cams + 2)) bad |= 1;           // first camera sees it
    if (IsVisible(s, 5, cams, cams + 2)) bad |= 2;             // too far for both
    if (IsVisible(s, 50, cams, cams)) bad |= 4;                // no cameras
    s.Center = C3DVector(100, 10, 0);
    if (!IsVisible(s, 50, cams, cams + 2)) bad |= 8;           // only the second camera sees it
    s.Center = C3DVector(50, -10, 0);
    if (IsVisible(s, 500, cams, cams + 2)) bad |= 16;          // behind both
    if (bad) { std::printf("00b38ed0_TEST FAIL\n"); return 1; }
    std::printf("00b38ed0_TEST PASS\n");
    return 0;
}