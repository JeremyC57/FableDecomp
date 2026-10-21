// behaviour test for 00b900b0 CEnginePrimitiveManagerOcclusionBase::IsClippedByNearPlane: a box is clipped when any of its
// eight corners, taken through box * view, has camera depth below the near distance
#include <cstdio>
struct V { float X, Y, Z; };
struct M { V Right, Up, Forward, Pos; };
static void Multiply(M& r, const M& a, const M& b)
{
    const V* ra[4] = { &a.Right, &a.Up, &a.Forward, &a.Pos };
    V* rr[4] = { &r.Right, &r.Up, &r.Forward, &r.Pos };
    for (int i = 0; i < 4; ++i) {
        rr[i]->X = ra[i]->X * b.Right.X + ra[i]->Y * b.Up.X + ra[i]->Z * b.Forward.X + (i == 3 ? b.Pos.X : 0.0f);
        rr[i]->Y = ra[i]->X * b.Right.Y + ra[i]->Y * b.Up.Y + ra[i]->Z * b.Forward.Y + (i == 3 ? b.Pos.Y : 0.0f);
        rr[i]->Z = ra[i]->X * b.Right.Z + ra[i]->Y * b.Up.Z + ra[i]->Z * b.Forward.Z + (i == 3 ? b.Pos.Z : 0.0f);
    }
}
static bool IsClippedByNearPlane(const M& box, const M& view, float nearClip)
{
    V corners[8] = { { -1, -1, -1 }, { -1, -1, 1 }, { -1, 1, -1 }, { -1, 1, 1 }, { 1, -1, -1 }, { 1, -1, 1 }, { 1, 1, -1 }, { 1, 1, 1 } };
    M toCamera;
    Multiply(toCamera, box, view);
    for (unsigned long i = 0; i < 8; ++i) {
        const V& c = corners[i];
        if (c.X * toCamera.Right.Z + c.Y * toCamera.Up.Z + c.Z * toCamera.Forward.Z + toCamera.Pos.Z < nearClip)
            return true;
    }
    return false;
}
int main()
{
    M view = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } };
    M box = { { 2, 0, 0 }, { 0, 2, 0 }, { 0, 0, 2 }, { 0, 0, 10 } };   // depth 8..12
    if (IsClippedByNearPlane(box, view, 1.0f)) { std::printf("00b900b0_TEST FAIL\n"); return 1; }
    if (!IsClippedByNearPlane(box, view, 9.0f)) { std::printf("00b900b0_TEST FAIL\n"); return 1; }
    view.Pos.Z = -7.5f;                                               // depth 0.5..4.5
    if (!IsClippedByNearPlane(box, view, 1.0f)) { std::printf("00b900b0_TEST FAIL\n"); return 1; }
    std::printf("00b900b0_TEST PASS\n");
    return 0;
}