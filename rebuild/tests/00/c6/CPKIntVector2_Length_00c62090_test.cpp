// behaviour test for 0x00C62090 CPKIntVector2::Length: sqrt(x*x + y*y)
#include <cstdio>
#include <cmath>
struct CPKIntVector2 {
    int x, y;
    float Length() const { return (float)std::sqrt((double)(x * x + y * y)); }
};
int main()
{
    CPKIntVector2 a = { 3, 4 }, b = { -5, 12 }, c = { 0, 0 }, d = { 1, 1 };
    if (std::fabs(a.Length() - 5.0f) > 1e-6f || std::fabs(b.Length() - 13.0f) > 1e-6f || c.Length() != 0.0f
        || std::fabs(d.Length() - 1.4142135f) > 1e-6f) { std::printf("00c62090_TEST FAIL\n"); return 1; }
    std::printf("00c62090_TEST PASS\n");
    return 0;
}