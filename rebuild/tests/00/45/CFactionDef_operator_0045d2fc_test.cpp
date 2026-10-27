// behaviour test for 0x0045D2FC: assignment copies every member and returns *this
#include <cstdio>
#include <cstring>
struct D { long a; bool b; long c; float d; };
int main()
{
    D x; D y; std::memset(&y, 0, sizeof(y)); x.a = 5; x.b = true; x.c = -3; x.d = 2.5f;
    D& r = (y = x);
    if (&r != &y || y.a != 5 || !y.b || y.c != -3 || y.d != 2.5f) { std::printf("0045d2fc_TEST FAIL\n"); return 1; }
    std::printf("0045d2fc_TEST PASS\n");
    return 0;
}