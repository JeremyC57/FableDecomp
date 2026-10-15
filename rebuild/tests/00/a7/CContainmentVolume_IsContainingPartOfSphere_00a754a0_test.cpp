#include <cstdio>
// Behaviour test for CContainmentVolume::IsContainingPartOfSphere (retail 0x00A754A0): the functor carries the point and
// radius unchanged into the IsContaining<CIsContainingPartOfSphere> plane test.
struct C3DVector { float X, Y, Z; };
struct CPlane { C3DVector Normal; float Distance; };

struct CIsContainingPartOfSphere {
    C3DVector Point;
    float     Radius;
    CIsContainingPartOfSphere(const C3DVector& point, float radius) : Point(point), Radius(radius) { }
};

struct CContainmentVolume_Methods {
    void*         __vftable;
    CPlane*       ContainmentPlanes;
    unsigned long Size;
    void*         MemoryPool;
    // Stand-in for the retail template (0x00A75790 / 0x00A757F0): every plane must see the
    // sphere's centre less than Radius behind it (sphere) / at least partially in front (part).
    bool IsContaining(const CIsContainingPartOfSphere& t) const
    {
        for (unsigned long i = 0; i < Size; ++i) {
            const CPlane& p = ContainmentPlanes[i];
            float d = t.Point.Z * p.Normal.Z + t.Point.Y * p.Normal.Y + t.Point.X * p.Normal.X - p.Distance;
            if (0 ? !(d > t.Radius) : !(d > -t.Radius)) return false;
        }
        return true;
    }
    bool IsContainingPartOfSphere(const C3DVector& point, float radius) const;
};

bool CContainmentVolume_Methods::IsContainingPartOfSphere(const C3DVector& point, float radius) const
{
    return IsContaining(CIsContainingPartOfSphere(point, radius));
}

int main()
{
    CPlane planes[2];
    planes[0].Normal.X = 1.0f; planes[0].Normal.Y = 0.0f; planes[0].Normal.Z = 0.0f; planes[0].Distance = 0.0f;   // x > 0
    planes[1].Normal.X = -1.0f; planes[1].Normal.Y = 0.0f; planes[1].Normal.Z = 0.0f; planes[1].Distance = -10.0f; // x < 10
    CContainmentVolume_Methods v;
    v.__vftable = 0; v.ContainmentPlanes = planes; v.Size = 2; v.MemoryPool = 0;
    C3DVector c; c.X = 5.0f; c.Y = 1.0f; c.Z = 2.0f;
    if (!v.IsContainingPartOfSphere(c, 1.0f)) { std::printf("00a754a0_TEST FAIL\n"); return 1; }
    c.X = 30.0f;
    if (v.IsContainingPartOfSphere(c, 1.0f)) { std::printf("00a754a0_TEST FAIL\n"); return 1; }
    c.X = 9.5f;   // radius decides: 2.0 overlaps the far plane
    if (v.IsContainingPartOfSphere(c, 2.0f) != (0 ? false : true)) { std::printf("00a754a0_TEST FAIL\n"); return 1; }
    v.Size = 0;
    if (!v.IsContainingPartOfSphere(c, 2.0f)) { std::printf("00a754a0_TEST FAIL\n"); return 1; }
    std::printf("00a754a0_TEST PASS\n");
    return 0;
}