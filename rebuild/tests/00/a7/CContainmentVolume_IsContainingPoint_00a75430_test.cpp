#include <cstdio>
struct CContainmentVolume { void* __vftable; void* ContainmentPlanes; unsigned long Size; void* MemoryPool; };
struct C3DVector { float X; float Y; float Z; };
struct CIsContainingPoint {
    C3DVector Point;
    CIsContainingPoint(const C3DVector& point) : Point(point) { }
};
struct CContainmentVolume_Methods : CContainmentVolume {
    bool IsContaining(const CIsContainingPoint& functor) const
    {
        const float* limit = static_cast<const float*>(ContainmentPlanes);
        return functor.Point.X < *limit && functor.Point.Y < *limit && functor.Point.Z < *limit;
    }
    bool IsContainingPoint(const C3DVector& point) const;
};
bool CContainmentVolume_Methods::IsContainingPoint(const C3DVector& point) const
{
    return IsContaining(CIsContainingPoint(point));
}
int main()
{
    float limit = 2.0f;
    CContainmentVolume_Methods v; v.ContainmentPlanes = &limit; v.Size = 1;
    C3DVector in = { 1.0f, 1.5f, -3.0f }, out = { 1.0f, 2.5f, 0.0f };
    if (!v.IsContainingPoint(in) || v.IsContainingPoint(out)) { std::printf("00a75430_TEST FAIL\n"); return 1; }
    std::printf("00a75430_TEST PASS\n");
    return 0;
}