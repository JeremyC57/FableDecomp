// Behaviour test for 0x0084eaf0: CTools::GetRollingAverage<C3DVector>: (average * count + value) / (count + 1)
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct V { float x, y, z; V() {} V(float a, float b, float c) : x(a), y(b), z(c) {} V operator*(float f) const { return V(x * f, y * f, z * f); } V operator+(const V& o) const { return V(x + o.x, y + o.y, z + o.z); } V operator/(float f) const { float inv = 1.0f / f; return V(x * inv, y * inv, z * inv); } };
static V Avg(const V& a, const V& v, float n) { return (a * n + v) / (n + 1.0f); }
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static void run() { V r = Avg(V(2, 4, 6), V(6, 0, -6), 3.0f); CHECK(Near(r.x, 3.0f) && Near(r.y, 3.0f) && Near(r.z, 3.0f)); V s = Avg(V(1, 1, 1), V(5, 5, 5), 0.0f); CHECK(Near(s.x, 5.0f)); }

int main()
{
    run();
    if (g_fail) { std::printf("0084eaf0_TEST FAIL\n"); return 1; }
    std::printf("0084eaf0_TEST PASS\n");
    return 0;
}
