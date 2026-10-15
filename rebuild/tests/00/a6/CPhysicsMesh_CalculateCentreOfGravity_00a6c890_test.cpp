#include <cstdio>
struct V { float x, y, z; V& operator*=(float s) { x *= s; y *= s; z *= s; return *this; } };
struct Add { V* sum; Add(V& s) : sum(&s) {} void operator()(const V& v) { sum->x += v.x; sum->y += v.y; sum->z += v.z; } };
template <class It, class F> F ForEach(It f, It l, F fn) { for (; f != l; ++f) fn(*f); return fn; }
struct Mesh { V* first; V* last; V cog;
    void Calc() { V& c = cog; c.x = c.y = c.z = 0.0f; ForEach(first, last, Add(c)); c *= 1.0f / (unsigned long)(last - first); } };
int main()
{
    V pts[2] = { { 1.0f, 2.0f, 3.0f }, { 3.0f, 4.0f, 5.0f } };
    Mesh m; m.first = pts; m.last = pts + 2; m.cog.x = 99.0f; m.Calc();
    if (m.cog.x != 2.0f || m.cog.y != 3.0f || m.cog.z != 4.0f) { std::printf("00a6c890_TEST FAIL\n"); return 1; }
    std::printf("00a6c890_TEST PASS\n");
    return 0;
}