#include <cstdio>
#include <cstring>
static int g_live = 0; static long g_kind = -1; static char g_name[64]; static void* g_o = 0; static void* g_t = 0;
struct S { const char* p; S(const char* t, long) : p(t) { ++g_live; } ~S() { --g_live; } };
struct Base { virtual ~Base() {} unsigned char body[0xb0]; Base(void* o, void* t, const S& n, long k) { g_o = o; g_t = t; std::strcpy(g_name, n.p); g_kind = k; } };
struct Act : Base { Act(void* o, void* t) : Base(o, t, S("TAKE_FOOD_FROM_TABLE", -1), 0) {} };
int main()
{
    int o, t; Act a(&o, &t);
    if (g_o != &o || g_t != &t || g_kind != 0 || g_live != 0 || std::strcmp(g_name, "TAKE_FOOD_FROM_TABLE")) { std::printf("00922ee0_TEST FAIL\n"); return 1; }
    std::printf("00922ee0_TEST PASS\n");
    return 0;
}