// behaviour test for 0x004E1132: assignment copies every member and returns *this
#include <cstdio>
#include <cstring>
struct D { long a; bool b; long c; float d; };
int main()
{
    D x; D y; std::memset(&y, 0, sizeof(y)); x.a = 5; x.b = true; x.c = -3; x.d = 2.5f;
    D& r = (y = x);
    if (&r != &y || y.a != 5 || !y.b || y.c != -3 || y.d != 2.5f) { std::printf("004e1132_TEST FAIL\n"); return 1; }
    std::printf("004e1132_TEST PASS\n");
    return 0;
}