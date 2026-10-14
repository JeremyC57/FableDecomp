#include <cstdio>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
struct S { int id; };
static long g_code = 0; static int g_name = 0;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(void* o, long c, const CSoundPair& s) { g_code = c; } void SetAnimationName(const S& n) { g_name = n.id; } };
struct Act : Base { unsigned char a, b, c; Act(void* o, const S& n, bool f) : Base(o, 50, CSoundPair()), a(0), b(f), c(0) { SetAnimationName(n); } };
int main()
{
    S n = { 42 }; Act x(0, n, true);
    if (g_code != 50 || g_name != 42 || x.a != 0 || x.b != 1 || x.c != 0) { std::printf("008b7ff0_TEST FAIL\n"); return 1; }
    std::printf("008b7ff0_TEST PASS\n");
    return 0;
}