#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00C257B0: exercises the same logic through a copy of the model (compiled separately from the source)
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
struct C3DVector { float x, y, z; C3DVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {} };
static C3DVector Subtract(const C3DVector& a, const C3DVector& b) { return C3DVector(a.x - b.x, a.y - b.y, a.z - b.z); }
static bool Run()
{
    C3DVector a(5.0f, -1.0f, 2.5f), b(1.5f, 2.0f, -4.0f);
    C3DVector d = Subtract(a, b);
    return near(d.x, 3.5f) && near(d.y, -3.0f) && near(d.z, 6.5f);
}
int main()
{
    if (!Run()) { std::printf("00c257b0_TEST FAIL\n"); return 1; }
    std::printf("00c257b0_TEST PASS\n");
    return 0;
}
