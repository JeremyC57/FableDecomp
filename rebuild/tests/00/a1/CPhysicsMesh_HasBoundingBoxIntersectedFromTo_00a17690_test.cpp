// Behaviour test for 0x00A17690 CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from,
// const C3DVector& to) const: a model of the row; the mesh box (centre/half-size from Min/Max) overlaps, in X and Y, the box
// around `from` grown by the radius plus |to - from| + 50 on each axis.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Mesh {
    V Min, Max;
    bool HasBoundingBoxIntersectedFromTo(float radius, const V& from, const V& to) const
    {
        V centre((Max.X + Min.X) * 0.5f, (Max.Y + Min.Y) * 0.5f, (Max.Z + Min.Z) * 0.5f);
        V halfSize((Max.X - Min.X) * 0.5f, (Max.Y - Min.Y) * 0.5f, (Max.Z - Min.Z) * 0.5f);
        V delta(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
        float dx = (float)std::fabs(delta.X) + 50.0f;
        float dy = (float)std::fabs(delta.Y) + 50.0f;
        if (from.X - radius - dx < centre.X + halfSize.X &&
            from.Y - radius - dy < centre.Y + halfSize.Y &&
            from.X + radius + dx > centre.X - halfSize.X &&
            from.Y + radius + dy > centre.Y - halfSize.Y)
            return true;
        return false;
    }
};
int main()
{
    Mesh m; m.Min = V(0, 0, 0); m.Max = V(10, 10, 10);
    int ok = 1;
    ok &= m.HasBoundingBoxIntersectedFromTo(1.0f, V(5, 5, 0), V(5, 5, 0));          // inside
    ok &= m.HasBoundingBoxIntersectedFromTo(1.0f, V(60, 5, 0), V(60, 5, 0));        // 60-1-50 = 9 < 10: overlaps
    ok &= !m.HasBoundingBoxIntersectedFromTo(1.0f, V(62, 5, 0), V(62, 5, 0));       // 11 > 10: misses in X
    ok &= !m.HasBoundingBoxIntersectedFromTo(1.0f, V(5, -52, 0), V(5, -52, 0));     // -52+1+50 = -1 < 0: misses in Y
    ok &= m.HasBoundingBoxIntersectedFromTo(1.0f, V(70, 5, 0), V(60, 5, 0));        // the from-to distance widens the box
    std::printf(ok ? "00a17690_TEST PASS\n" : "00a17690_TEST FAIL\n");
    return ok ? 0 : 1;
}