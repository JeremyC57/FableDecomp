// Behaviour test for 00ade080 (C3DBoundingBox::InitialiseFromOBB): a standalone copy of the x87 arm -- the four half-extent
// corners (hx,hy,hz), (hx,-hy,hz), (-hx,hy,hz), (-hx,-hy,hz) rotated by the 3x3, the centre transformed with the translation, and
// the axis-aligned box of centre +/- each corner.
#include <cstdio>
#include <cmath>
struct V3 { float x, y, z; };
struct M { float e[4][3]; };
static V3 rot(const M& m, V3 v) { V3 r = { v.x * m.e[0][0] + v.y * m.e[1][0] + v.z * m.e[2][0], v.x * m.e[0][1] + v.y * m.e[1][1] + v.z * m.e[2][1], v.x * m.e[0][2] + v.y * m.e[1][2] + v.z * m.e[2][2] }; return r; }
static void Obb(V3 mn, V3 mx, const M& m, V3& omin, V3& omax)
{
    V3 h = { (mx.x - mn.x) * 0.5f, (mx.y - mn.y) * 0.5f, (mx.z - mn.z) * 0.5f };
    V3 v[4] = { { h.x, h.y, h.z }, { h.x, -h.y, h.z }, { -h.x, h.y, h.z }, { -h.x, -h.y, h.z } };
    for (int i = 0; i < 4; ++i) v[i] = rot(m, v[i]);
    V3 c = { (mn.x + mx.x) * 0.5f, (mn.y + mx.y) * 0.5f, (mn.z + mx.z) * 0.5f };
    c = rot(m, c); c.x += m.e[3][0]; c.y += m.e[3][1]; c.z += m.e[3][2];
    omin = omax = c;
    for (int i = 0; i < 4; ++i) for (int s = -1; s <= 1; s += 2) {
        V3 p = { c.x + s * v[i].x, c.y + s * v[i].y, c.z + s * v[i].z };
        if (omin.x > p.x) omin.x = p.x; if (omin.y > p.y) omin.y = p.y; if (omin.z > p.z) omin.z = p.z;
        if (omax.x < p.x) omax.x = p.x; if (omax.y < p.y) omax.y = p.y; if (omax.z < p.z) omax.z = p.z;
    }
}
int main()
{
    int fails = 0; V3 a, b;
    M id = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 10, 0, 0 } } };
    V3 mn = { -1, -2, -3 }, mx = { 1, 2, 3 };
    Obb(mn, mx, id, a, b);
    if (a.x != 9 || a.y != -2 || a.z != -3 || b.x != 11 || b.y != 2 || b.z != 3) ++fails;
    M rz = { { { 0, 1, 0 }, { -1, 0, 0 }, { 0, 0, 1 }, { 0, 0, 0 } } };   // 90 degrees about Z swaps the X / Y extents
    Obb(mn, mx, rz, a, b);
    if (a.x != -2 || b.x != 2 || a.y != -1 || b.y != 1) ++fails;
    if (fails) { std::printf("00ade080_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ade080_TEST PASS\n");
    return 0;
}
