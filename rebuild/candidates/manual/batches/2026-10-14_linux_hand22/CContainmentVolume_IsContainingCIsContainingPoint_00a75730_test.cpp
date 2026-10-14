#include <cstdio>
struct CContainmentVolume { void* __vftable; void* ContainmentPlanes; unsigned long Size; void* MemoryPool; };
struct C3DVector { float X; float Y; float Z; };
struct CPlane {
    C3DVector Normal;
    float     Distance;
    __forceinline float GetSignedDistance(const C3DVector& point) const
    {
        return point.Z * Normal.Z + point.Y * Normal.Y + point.X * Normal.X - Distance;
    }
};
struct CIsContainingPoint {
    C3DVector Point;
    __forceinline bool operator()(const CPlane& plane) const { return plane.GetSignedDistance(Point) > 0.0f; }
};
struct CContainmentVolume_Methods : CContainmentVolume {
    template<class TTest> bool IsContaining(const TTest& test) const;
};
template<class TTest>
bool CContainmentVolume_Methods::IsContaining(const TTest& test) const
{
    bool contained = true;
    unsigned long i = 0;
    while (contained && i < Size) {
        if (!test(reinterpret_cast<const CPlane*>(ContainmentPlanes)[i]))
            contained = false;
        else
            ++i;
    }
    return contained;
}
// the unit cube [-1,1]^3 with inward normals: signed distance = N.P - D is positive inside
static CPlane g_cube[6] = {
    { { 1, 0, 0 }, -1 }, { { -1, 0, 0 }, -1 },
    { { 0, 1, 0 }, -1 }, { { 0, -1, 0 }, -1 },
    { { 0, 0, 1 }, -1 }, { { 0, 0, -1 }, -1 },
};
int main()
{
    CContainmentVolume_Methods v; v.ContainmentPlanes = g_cube; v.Size = 6;
    CIsContainingPoint inside = { { 0.5f, -0.5f, 0.25f } }, outside = { { 0.5f, 1.5f, 0.0f } }, edge = { { 1.0f, 0.0f, 0.0f } };
    if (!v.IsContaining(inside) || v.IsContaining(outside) || v.IsContaining(edge)) { std::printf("00a75730_TEST FAIL\n"); return 1; }
    v.Size = 0;
    if (!v.IsContaining(outside)) { std::printf("00a75730_TEST FAIL\n"); return 1; }
    std::printf("00a75730_TEST PASS\n");
    return 0;
}
