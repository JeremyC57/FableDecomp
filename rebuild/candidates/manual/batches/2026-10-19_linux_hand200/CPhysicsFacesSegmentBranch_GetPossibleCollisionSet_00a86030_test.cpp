// Behaviour test for CPhysicsFacesSegmentBranch::GetPossibleCollisionSet (0x00A86030): visit each child whose half-space the inflated segment reaches.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CPlaneModel {
    C3DVector Normal; float Distance;
    float GetDistanceToPoint(const C3DVector& p) const { return Normal.X * p.X + Normal.Y * p.Y + Normal.Z * p.Z - Distance; }
};
struct Faces { int visits[2]; };
struct CIPhysicsFacesSegmentBase {
    int which;
    virtual void GetPossibleCollisionSet(float, const C3DVector&, const C3DVector&, Faces& faces) const { faces.visits[which]++; }
    virtual ~CIPhysicsFacesSegmentBase() {}
};
struct CPhysicsFacesSegmentBranch {
    CPlaneModel Plane; CIPhysicsFacesSegmentBase* PositiveChild; CIPhysicsFacesSegmentBase* NegativeChild;
    void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end, Faces& faces) const
    {
        float startDistance = Plane.GetDistanceToPoint(start);
        float endDistance = Plane.GetDistanceToPoint(end);
        if (startDistance < radius || endDistance < radius)
            NegativeChild->GetPossibleCollisionSet(radius, start, end, faces);
        if (startDistance > -radius || endDistance > -radius)
            PositiveChild->GetPossibleCollisionSet(radius, start, end, faces);
    }
};
static bool Check(const CPhysicsFacesSegmentBranch& b, float r, float z0, float z1, int neg, int pos)
{
    Faces f = { { 0, 0 } }; C3DVector a = { 0, 0, z0 }, c = { 0, 0, z1 };
    b.GetPossibleCollisionSet(r, a, c, f);
    return f.visits[0] == pos && f.visits[1] == neg;
}
int main()
{
    CIPhysicsFacesSegmentBase p, n; p.which = 0; n.which = 1;
    CPhysicsFacesSegmentBranch b; b.Plane.Normal.X = 0; b.Plane.Normal.Y = 0; b.Plane.Normal.Z = 1; b.Plane.Distance = 2; b.PositiveChild = &p; b.NegativeChild = &n;
    if (!Check(b, 0.5f, 5, 6, 0, 1) || !Check(b, 0.5f, -5, -6, 1, 0) || !Check(b, 0.5f, 1, 5, 1, 1) || !Check(b, 0.5f, 2.2f, 9, 1, 1) || !Check(b, 0.5f, 1.4f, -3, 1, 0)) { std::printf("00a86030_TEST FAIL\n"); return 1; }
    std::printf("00a86030_TEST PASS\n");
    return 0;
}
