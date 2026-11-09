// Behaviour test for CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace 0x00BBA3E0: the sphere is centred on the
// decal's world position with radius |Size| (sqrt(x*x + y*y)), every sphere word is overwritten, and the result is true.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class C2DVector { public: float X, Y; };
class C3DBoundingSphere { public:
    C3DVector Center; float Radius;
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DVector& centre, float radius) : Center(centre), Radius(radius) { }
};
class CEngineInternalPrimitiveDecal {
public:
    C3DVector WorldPos;
    C2DVector Size;
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
    {
        float x = Size.X;
        float y = Size.Y;
        sphere = C3DBoundingSphere(WorldPos, sqrtf((x * x) + (y * y)));
        return true;
    }
};
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
static int fail(int n) { std::printf("00bba3e0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CEngineInternalPrimitiveDecal d;
    d.WorldPos.X = 10.0f; d.WorldPos.Y = -2.5f; d.WorldPos.Z = 7.0f; d.Size.X = 3.0f; d.Size.Y = 4.0f;
    C3DBoundingSphere s; s.Center.X = s.Center.Y = s.Center.Z = s.Radius = -99.0f;
    if (!d.GetBoundingSphereWorldSpace(s)) return fail(1);
    if (!nearly(s.Center.X, 10.0f) || !nearly(s.Center.Y, -2.5f) || !nearly(s.Center.Z, 7.0f) || !nearly(s.Radius, 5.0f)) return fail(2);
    d.Size.X = -6.0f; d.Size.Y = 0.0f;
    if (!d.GetBoundingSphereWorldSpace(s) || !nearly(s.Radius, 6.0f)) return fail(3);
    d.Size.X = 0.0f;
    if (!d.GetBoundingSphereWorldSpace(s) || !nearly(s.Radius, 0.0f)) return fail(4);
    std::printf("00bba3e0_TEST PASS\n");
    return 0;
}