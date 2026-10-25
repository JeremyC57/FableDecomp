// Behaviour test for 0x00B96350 CShadowClipVolume::ClipSphere(const C3DBoundingSphere&): false when the sphere misses
// the volume's own bounding sphere (if ClipToSphere) or lies wholly in front of any clip plane, else true.
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float Dot(const C3DVector& v) const { float dot = Z * v.Z + X * v.X; dot += Y * v.Y; return dot; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CPlane { C3DVector Normal; float D; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct CShadowClipVolume {
    CPlane ClipPlanes[8]; long ClipPlaneCount; C3DBoundingSphere BoundingSphere; bool ClipToSphere;
    bool ClipSphere(const C3DBoundingSphere& sphere) const
    {
        float radius = sphere.Radius;
        C3DVector centre = sphere.Center;
        if (ClipToSphere) {
            C3DVector d = centre - BoundingSphere.Center;
            float r = radius + BoundingSphere.Radius;
            if (d.GetLengthSquared() > r * r)
                return false;
        }
        for (long i = 0; i < ClipPlaneCount; ++i)
            if (centre.Dot(ClipPlanes[i].Normal) - radius > ClipPlanes[i].D)
                return false;
        return true;
    }
};

static C3DBoundingSphere Sphere(float x, float y, float z, float r) { C3DBoundingSphere s; s.Center = C3DVector(x, y, z); s.Radius = r; return s; }

int main()
{
    CShadowClipVolume v;
    v.ClipPlaneCount = 2;
    v.ClipPlanes[0].Normal = C3DVector(1.0f, 0.0f, 0.0f); v.ClipPlanes[0].D = 10.0f;    // reject x - r > 10
    v.ClipPlanes[1].Normal = C3DVector(0.0f, 0.0f, -1.0f); v.ClipPlanes[1].D = 5.0f;    // reject -z - r > 5
    v.BoundingSphere = Sphere(0.0f, 0.0f, 0.0f, 20.0f);
    v.ClipToSphere = false;
    int ok = 1;
    if (!v.ClipSphere(Sphere(0.0f, 0.0f, 0.0f, 1.0f))) ok = 0;
    if (v.ClipSphere(Sphere(12.0f, 0.0f, 0.0f, 1.0f))) ok = 0;          // 11 > 10
    if (!v.ClipSphere(Sphere(12.0f, 0.0f, 0.0f, 3.0f))) ok = 0;         // 9 <= 10
    if (v.ClipSphere(Sphere(0.0f, 3.0f, -8.0f, 2.0f))) ok = 0;          // 6 > 5
    if (!v.ClipSphere(Sphere(0.0f, 50.0f, 0.0f, 1.0f))) ok = 0;         // far but no sphere test
    v.ClipToSphere = true;
    if (v.ClipSphere(Sphere(0.0f, 50.0f, 0.0f, 1.0f))) ok = 0;          // 50^2 > 21^2
    if (!v.ClipSphere(Sphere(0.0f, 20.0f, 0.0f, 1.0f))) ok = 0;         // 400 <= 441
    std::printf(ok ? "00b96350_TEST PASS\n" : "00b96350_TEST FAIL\n");
    return ok ? 0 : 1;
}