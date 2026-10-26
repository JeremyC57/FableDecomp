// behaviour test for 00baa4e0 CEngineInternalPrimitiveRepeatedZSprite::IsInsideFrustum
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
class CPlane { public: C3DVector Normal; float D; };
class CCamera { public: C3DVector WorldPosition; float NearZPlane, FarZPlane; C3DVector LookVector; CPlane FrustrumPlanes[4]; };
struct CSprite { float FadeFar; float FadeInStart; C3DVector Center; float Radius; };
static float g_fadeScale = 1.0f;
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
static bool IsInsideFrustum(const CSprite* self, const CCamera& camera, bool ignoreFrustum)
{
    float dist = (self->Center - camera.WorldPosition).GetLength();
    if (dist - self->Radius > g_fadeScale * self->FadeFar) return false;
    if (dist + self->Radius < self->FadeInStart) return false;
    if (ignoreFrustum) return true;
    return IsSphereInFrustum(camera, self->Center, self->Radius);
}
int main()
{
    CCamera cam;
    cam.WorldPosition = C3DVector(0, 0, 0); cam.NearZPlane = 1; cam.FarZPlane = 100; cam.LookVector = C3DVector(0, 1, 0);
    // planes facing outwards: left (-x), right (+x), bottom (-z), top (+z), 45 degrees
    float k = 0.70710678f;
    cam.FrustrumPlanes[0].Normal = C3DVector(-k, -k, 0); cam.FrustrumPlanes[1].Normal = C3DVector(k, -k, 0);
    cam.FrustrumPlanes[2].Normal = C3DVector(0, -k, -k); cam.FrustrumPlanes[3].Normal = C3DVector(0, -k, k);
    CSprite s; s.FadeFar = 50; s.FadeInStart = 2; s.Radius = 1;
    int bad = 0;
    s.Center = C3DVector(0, 10, 0); if (!IsInsideFrustum(&s, cam, false)) bad |= 1;        // straight ahead
    s.Center = C3DVector(0, 60, 0); if (IsInsideFrustum(&s, cam, false)) bad |= 2;         // beyond the fade distance
    g_fadeScale = 2.0f; if (!IsInsideFrustum(&s, cam, false)) bad |= 4; g_fadeScale = 1.0f; // scale extends it
    s.Center = C3DVector(0, 0.5f, 0); if (IsInsideFrustum(&s, cam, true)) bad |= 8;         // before the fade-in start
    s.Center = C3DVector(-30, 10, 0); if (IsInsideFrustum(&s, cam, false)) bad |= 16;       // left of the frustum
    if (!IsInsideFrustum(&s, cam, true)) bad |= 32;                                         // frustum test skipped
    s.Center = C3DVector(0, 10, 30); if (IsInsideFrustum(&s, cam, false)) bad |= 64;        // above
    s.Center = C3DVector(0, -10, 0); if (IsInsideFrustum(&s, cam, false)) bad |= 128;       // behind the near plane
    if (bad) { std::printf("00baa4e0_TEST FAIL\n"); return 1; }
    std::printf("00baa4e0_TEST PASS\n");
    return 0;
}