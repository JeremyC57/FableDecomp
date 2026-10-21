#include <cstdio>
int __stdcall Sign(float value)
{
    return value < 0.0f ? -1 : 1;
}

int main()
{
    float zero = 0.0f;
    float nan = zero / zero;
    bool ok = Sign(-2.5f) == -1 && Sign(-0.0001f) == -1 && Sign(0.0f) == 1 && Sign(-0.0f) == 1
           && Sign(3.0f) == 1 && Sign(nan) == 1;
    if (ok) printf("00437160_TEST PASS\n"); else printf("FAIL\n");
    return 0;
}