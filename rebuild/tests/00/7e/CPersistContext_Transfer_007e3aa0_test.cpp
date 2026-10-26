#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, long&): forwards to the three-argument transfer with a -1 default
static const char* g_name; static long* g_value; static long g_default; static long g_calls;
class CPersistContext { public:
    long pad[11];
    void Transfer_007e39d0(const char* name, long& value, const long& defaultValue) { g_name = name; g_value = &value; g_default = defaultValue; ++g_calls; }
    void Transfer(const char* name, long& value) { Transfer_007e39d0(name, value, -1L); }
};
int main()
{
    CPersistContext c; long v = 7;
    c.Transfer("Index", v);
    if (g_calls != 1 || std::strcmp(g_name, "Index") != 0 || g_value != &v || g_default != -1 || v != 7) { std::printf("007e3aa0_TEST FAIL\n"); return 1; }
    std::printf("007e3aa0_TEST PASS\n");
    return 0;
}