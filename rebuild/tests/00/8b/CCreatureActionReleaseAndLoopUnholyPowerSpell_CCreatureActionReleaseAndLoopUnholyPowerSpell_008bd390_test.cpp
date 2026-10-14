#include <cstdio>
#include <cstring>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static int g_live = 0; static long g_code = 0; static char g_name[64];
struct S { const char* p; S(const char* t, long) : p(t) { ++g_live; } ~S() { --g_live; } };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(void* o, long c, const CSoundPair& s) { g_code = c; } void SetAnimationName(const S& n) { std::strcpy(g_name, n.p); } };
struct Mid : Base { Mid(void* o, const S& n) : Base(o, 150, CSoundPair()) { SetAnimationName(n); } };
struct Act : Mid { Act(void* o) : Mid(o, S("ST_RELEASE_LOOP_UNHOLY_POWER", -1)) {} };
int main()
{
    Act a(0);
    if (g_code != 150 || g_live != 0 || std::strcmp(g_name, "ST_RELEASE_LOOP_UNHOLY_POWER")) { std::printf("008bd390_TEST FAIL\n"); return 1; }
    std::printf("008bd390_TEST PASS\n");
    return 0;
}