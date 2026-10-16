// behaviour test for C2DVector::ToString (0x00A13CE0)
#include <cstdio>
#include <cstring>
struct V2 { float X, Y; };
static void ToString(const V2& v, char* out) { std::sprintf(out, "(%.2f, %.2f)", (double)v.X, (double)v.Y); }
int main()
{
    V2 v = { 1.5f, -2.25f }; char buf[64];
    ToString(v, buf);
    if (std::strcmp(buf, "(1.50, -2.25)") != 0) { std::printf("00a13ce0_TEST FAIL\n"); return 1; }
    std::printf("00a13ce0_TEST PASS\n");
    return 0;
}