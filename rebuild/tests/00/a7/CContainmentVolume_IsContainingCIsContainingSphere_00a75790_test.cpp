#include <cstdio>
class CContainmentVolume { public: void* __vftable; void* ContainmentPlanes; unsigned long Size; void* MemoryPool; };
class C3DVector { public: float X; float Y; float Z; };
class CPlane { public:
    C3DVector Normal;
    float     Distance;
    __forceinline float GetSignedDistance(const C3DVector& point) const
    {
        return point.Z * Normal.Z + point.Y * Normal.Y + point.X * Normal.X - Distance;
    }
};
class CIsContainingSphere { public:
    C3DVector Point;
    float     Radius;
    __forceinline bool operator()(const CPlane& plane) const { return plane.GetSignedDistance(Point) > -Radius; }
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
    CIsContainingSphere overlapping = { { 1.4f, 0.0f, 0.0f }, 0.5f }, apart = { { 1.6f, 0.0f, 0.0f }, 0.5f }, inside = { { 0.0f, 0.0f, 0.0f }, 0.5f };
    if (!v.IsContaining(overlapping) || v.IsContaining(apart) || !v.IsContaining(inside)) { std::printf("00a75790_TEST FAIL\n"); return 1; }
    std::printf("00a75790_TEST PASS\n");
    return 0;
}