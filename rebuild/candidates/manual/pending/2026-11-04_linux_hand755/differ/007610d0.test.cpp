// Behaviour test for NShape::CSubdivisionTriangle::IsPosInShape (0x007610D0), g++ model.
#include <cstdio>
#include <vector>
struct V { float X, Y, Z; };
static bool IsPosInShape(const std::vector<V>& pts, V up, V pos)
{
    for (size_t i = 0; i < pts.size(); ++i) {
        const V& a = pts[i]; const V& b = pts[(i + 1) % pts.size()];
        V e = { b.X - a.X, b.Y - a.Y, 0.0f };
        V side = { e.Y * up.Z - e.Z * up.Y, e.Z * up.X - e.X * up.Z, e.X * up.Y - e.Y * up.X };
        V d = { pos.X - a.X, pos.Y - a.Y, 0.0f };
        if (side.X * d.X + side.Y * d.Y + side.Z * d.Z < 0.0f) return false;
    }
    return true;
}
int main()
{
    std::vector<V> tri; V a = { 0, 0, 5 }, b = { 0, 4, 1 }, c = { 4, 0, 2 }; tri.push_back(a); tri.push_back(b); tri.push_back(c);
    V up = { 0, 0, 1 };
    V in = { 1, 1, 9 }, out = { 5, 5, 0 }, edge = { 0, 2, 0 };
    int ok = IsPosInShape(tri, up, in) && !IsPosInShape(tri, up, out) && IsPosInShape(tri, up, edge);
    std::printf(ok ? "007610d0_TEST PASS\n" : "007610d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
