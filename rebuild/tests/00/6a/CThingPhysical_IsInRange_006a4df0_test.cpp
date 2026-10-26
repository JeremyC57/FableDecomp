#include <cstdio>
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    float GetLengthSquared() const { return x * x + y * y + z * z; }
};
class CTCPhysicsBase { public:
    void*         __vftable;
    unsigned char _pad04[0x8];
    C3DVector     Position;
};
class CThingPhysical { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;
    long IsInRange(const C3DVector& pos, float range);
};
long CThingPhysical::IsInRange(const C3DVector& pos, float range)
{
    C3DVector d = PhysicsTC->Position - pos;
    return d.GetLengthSquared() < range * range;
}
int main()
{
    CTCPhysicsBase tc; tc.Position = C3DVector(1.0f, 2.0f, 3.0f);
    CThingPhysical t; t.PhysicsTC = &tc;
    int bad = 0;
    if (t.IsInRange(C3DVector(1.0f, 2.0f, 8.0f), 5.0f) != 0) bad = 1;      /* exactly on the edge: outside */
    if (t.IsInRange(C3DVector(1.0f, 2.0f, 7.9f), 5.0f) != 1) bad = 2;
    if (t.IsInRange(C3DVector(4.0f, 6.0f, 3.0f), 4.9f) != 0) bad = 3;      /* distance 5 */
    if (t.IsInRange(C3DVector(4.0f, 6.0f, 3.0f), 5.1f) != 1) bad = 4;
    if (bad) { std::printf("006a4df0_TEST FAIL\n"); return 1; }
    std::printf("006a4df0_TEST PASS\n");
    return 0;
}