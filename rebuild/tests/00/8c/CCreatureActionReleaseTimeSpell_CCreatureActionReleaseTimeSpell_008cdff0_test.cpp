#include <cstdio>
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
struct S { int id; };
static long g_code = 0; static int g_name = 0; static void* g_owner = 0;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(void* o, long c, const CSoundPair& s) { g_code = c; g_owner = o; } void SetAnimationName(const S& n) { g_name = n.id; } };
struct Act : Base { long level; Act(long l, void* o, const S& n) : Base(o, 145, CSoundPair()), level(l) { SetAnimationName(n); } };
int main()
{
    S n = { 9 }; int o; Act x(3, &o, n);
    if (g_code != 145 || g_name != 9 || g_owner != &o || x.level != 3) { std::printf("008cdff0_TEST FAIL\n"); return 1; }
    std::printf("008cdff0_TEST PASS\n");
    return 0;
}