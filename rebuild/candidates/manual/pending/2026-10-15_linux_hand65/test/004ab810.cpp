#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, double&): forwards to the three-argument transfer with a 0.0 default
static const char* g_name; static double* g_value; static double g_default = -1.0; static long g_calls;
struct CPersistContext {
    long pad[11];
    void Transfer(const char* name, double& value, const double& defaultValue) { g_name = name; g_value = &value; g_default = defaultValue; ++g_calls; }
    void Transfer(const char* name, double& value) { Transfer(name, value, 0.0); }
};
int main()
{
    CPersistContext c; double v = 5.5;
    c.Transfer("Scale", v);
    if (g_calls != 1 || std::strcmp(g_name, "Scale") != 0 || g_value != &v || g_default != 0.0 || v != 5.5) { std::printf("004ab810_TEST FAIL\n"); return 1; }
    std::printf("004ab810_TEST PASS\n");
    return 0;
}
