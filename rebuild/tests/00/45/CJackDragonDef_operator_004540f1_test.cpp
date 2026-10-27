// behaviour test for 0x004540F1: assignment copies every member and returns *this
#include <cstdio>
#include <cstring>
struct D { long a; bool b; long c; float d; };
int main()
{
    D x; D y; std::memset(&y, 0, sizeof(y)); x.a = 5; x.b = true; x.c = -3; x.d = 2.5f;
    D& r = (y = x);
    if (&r != &y || y.a != 5 || !y.b || y.c != -3 || y.d != 2.5f) { std::printf("004540f1_TEST FAIL\n"); return 1; }
    std::printf("004540f1_TEST PASS\n");
    return 0;
}