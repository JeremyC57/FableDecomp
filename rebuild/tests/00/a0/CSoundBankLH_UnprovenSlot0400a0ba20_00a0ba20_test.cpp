// behaviour test for 0x00A0BA20: the returned value is zero in both words
#include <cstdio>
struct V { long a, b; };
static V Get() { V v = { 0, 0 }; return v; }
int main()
{
    V v = Get();
    if (v.a != 0 || v.b != 0) { std::printf("00a0ba20_TEST FAIL\n"); return 1; }
    std::printf("00a0ba20_TEST PASS\n");
    return 0;
}