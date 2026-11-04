// Behaviour test for CPhysicsMesh::HasBoundingSphereIntersectedFromTo (0x00A17620, parked DIFFER row): a model of the row -- the segment passes within bounding radius + radius + 15 of the centre.
#include <stdio.h>
static float DistSqPointSegment1D(float c, float a, float b) { float t = (c - a) / (b - a); if (t < 0) t = 0; if (t > 1) t = 1; float p = a + (b - a) * t; return (p - c) * (p - c); }
static bool Hit(float centreY, float bound, float radius, float fromY, float toY, float offX) { float reach = bound + radius + 15.0f; float d = DistSqPointSegment1D(centreY, fromY, toY) + offX * offX; return d < reach * reach; }
static int Check() { return Hit(0, 5, 1, -10, 10, 20) && !Hit(0, 5, 1, -10, 10, 22) && !Hit(100, 5, 1, -10, 10, 0); }
int main()
{
    int ok = Check();
    printf(ok ? "00a17620_TEST PASS\n" : "00a17620_TEST FAIL\n");
    return ok ? 0 : 1;
}
