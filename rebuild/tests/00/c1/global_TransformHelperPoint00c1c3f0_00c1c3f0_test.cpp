// Behaviour test for TransformHelperPoint_00c1c3f0 (0x00C1C3F0): point * 4x3 matrix rows + translation.
#include <cstdio>
class C3DVector { public: float x, y, z; };
struct C3DMatrix { C3DVector Row[3]; C3DVector Translation; };
static C3DVector TransformHelperPoint(const C3DMatrix& m, const C3DVector& p)
{
    C3DVector r;
    r.x = m.Row[2].x * p.z + m.Row[1].x * p.y + m.Row[0].x * p.x + m.Translation.x;
    r.y = m.Row[2].y * p.z + m.Row[0].y * p.x + m.Row[1].y * p.y + m.Translation.y;
    r.z = m.Row[2].z * p.z + m.Row[0].z * p.x + m.Row[1].z * p.y + m.Translation.z;
    return r;
}
int main()
{
    C3DMatrix m = { { { 1, 0, 0 }, { 0, 2, 0 }, { 0, 0, 3 } }, { 10, 20, 30 } };
    C3DVector p = { 1, 1, 1 };
    C3DVector r = TransformHelperPoint(m, p);
    if (r.x != 11 || r.y != 22 || r.z != 33) { std::printf("00c1c3f0_TEST FAIL\n"); return 1; }
    std::printf("00c1c3f0_TEST PASS\n"); return 0;
}