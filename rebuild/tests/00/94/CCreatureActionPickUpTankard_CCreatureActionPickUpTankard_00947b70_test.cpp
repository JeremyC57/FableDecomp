#include <cstdio>
#include <cstring>
static int g_live = 0; static long g_kind = -1; static char g_name[64]; static void* g_o = 0; static void* g_t = 0;
struct S { const char* p; S(const char* t, long) : p(t) { ++g_live; } ~S() { --g_live; } };
struct Base { virtual ~Base() {} unsigned char body[0xb0]; Base(void* o, void* t, const S& n, long k) { g_o = o; g_t = t; std::strcpy(g_name, n.p); g_kind = k; } };
struct Act : Base { Act(void* o, void* t) : Base(o, t, S("DRINK_AT_TABLE_INTO", -1), 39) {} };
int main()
{
    int o, t; Act a(&o, &t);
    if (g_o != &o || g_t != &t || g_kind != 39 || g_live != 0 || std::strcmp(g_name, "DRINK_AT_TABLE_INTO")) { std::printf("00947b70_TEST FAIL\n"); return 1; }
    std::printf("00947b70_TEST PASS\n");
    return 0;
}