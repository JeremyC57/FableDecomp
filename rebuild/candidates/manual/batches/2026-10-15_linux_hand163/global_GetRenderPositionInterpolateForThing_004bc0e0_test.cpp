// behaviour test for 0x004BC0E0 GetRenderPositionInterpolateForThing
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Interp { Vec StartPosition, EndPosition; };
struct Physics { Vec a, b; float GetHeightOffset(float t) { return t == 0.0f ? 1.0f : 3.0f; }
    void GetPositionInterpolator(Interp& o) const { o.StartPosition = a; o.EndPosition = b; } };
struct Thing { unsigned char Type; Physics* PPhysics; };
static Interp Get(const Thing& thing)
{
    Physics* p = thing.PPhysics;
    Interp i;
    p->GetPositionInterpolator(i);
    if (thing.Type == 2) { i.StartPosition.Z += p->GetHeightOffset(0.0f); i.EndPosition.Z += p->GetHeightOffset(1.0f); }
    return i;
}
int main()
{
    Physics p = { { 1, 2, 3 }, { 4, 5, 6 } };
    Thing t = { 2, &p };
    Interp i = Get(t);
    bool ok = i.StartPosition.Z == 4.0f && i.EndPosition.Z == 9.0f && i.StartPosition.X == 1.0f;
    t.Type = 1; i = Get(t);
    ok = ok && i.StartPosition.Z == 3.0f && i.EndPosition.Z == 6.0f;
    if (!ok) { std::printf("004bc0e0_TEST FAIL\n"); return 1; }
    std::printf("004bc0e0_TEST PASS\n");
    return 0;
}
