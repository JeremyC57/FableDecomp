#include <cstdio>
struct V { float X, Y, Z; };
struct Sphere { V Center; float Radius; };
struct Box { V Min, Max;
    bool IntersectsSolid(const Sphere& s) const {
        float d = 0.0f; float c[3] = { s.Center.X, s.Center.Y, s.Center.Z }; float lo[3] = { Min.X, Min.Y, Min.Z }, hi[3] = { Max.X, Max.Y, Max.Z };
        for (int i = 0; i < 3; ++i) { if (c[i] < lo[i]) { float e = c[i] - lo[i]; d += e * e; } else if (c[i] > hi[i]) { float e = c[i] - hi[i]; d += e * e; } }
        return d <= s.Radius * s.Radius; } };
int main() {
    Box b = { { 0, 0, 0 }, { 1, 1, 1 } };
    Sphere in = { { 0.5f, 0.5f, 0.5f }, 0.1f }, touch = { { 2, 0.5f, 0.5f }, 1.0f }, out = { { 2, 2, 0.5f }, 1.0f };
    if (!b.IntersectsSolid(in) || !b.IntersectsSolid(touch) || b.IntersectsSolid(out)) { std::printf("00adecc0_TEST FAIL\n"); return 1; }
    std::printf("00adecc0_TEST PASS\n"); return 0; }