// Behaviour model of NShape::IsShapeConvex (0x007602F0): sum the turning angles (in turns: acos(dot) / 2pi) between consecutive
// normalised XY edge directions of the closed outline (the two closing edges included); the outline is convex when the total is
// one full turn: (total - 1)^2 < 0.0025.
#include <cmath>
#include <cstdio>
#include <vector>
struct V { float X, Y, Z; };
static V Dir(const V& a, const V& b)
{
    V d = { b.X - a.X, b.Y - a.Y, 0.0f };
    float m = std::sqrt(d.X * d.X + d.Y * d.Y);
    if (m > 0.0f) { d.X /= m; d.Y /= m; }
    return d;
}
static float Turn(const V& e, const V& p) { return (float)std::acos(e.X * p.X + e.Y * p.Y) * (1.0f / 6.2831853f); }
static bool IsShapeConvex(const std::vector<V>& points)
{
    V previous = Dir(points[0], points[1]);
    float total = 0.0f;
    for (size_t i = 1; i + 1 < points.size(); ++i) {
        V e = Dir(points[i], points[i + 1]);
        total += Turn(e, previous);
        previous = e;
    }
    V closing = Dir(points.back(), points[0]);
    float closingTurn = Turn(closing, previous);
    V first = Dir(points[0], points[1]);
    total = Turn(first, closing) + (closingTurn + total);
    total -= 1.0f;
    return total * total < 0.0025f;
}
int main()
{
    V sq[] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0} };
    V hex[] = { {2, 0, 0}, {1, 1.7f, 0}, {-1, 1.7f, 0}, {-2, 0, 0}, {-1, -1.7f, 0}, {1, -1.7f, 0} };
    V ell[] = { {0, 0, 0}, {2, 0, 0}, {2, 1, 0}, {1, 1, 0}, {1, 2, 0}, {0, 2, 0} };
    if (!IsShapeConvex(std::vector<V>(sq, sq + 4))) { std::printf("007602f0_TEST FAIL square\n"); return 1; }
    if (!IsShapeConvex(std::vector<V>(hex, hex + 6))) { std::printf("007602f0_TEST FAIL hexagon\n"); return 1; }
    if (IsShapeConvex(std::vector<V>(ell, ell + 6))) { std::printf("007602f0_TEST FAIL L-shape\n"); return 1; }
    std::printf("007602f0_TEST PASS\n");
    return 0;
}
