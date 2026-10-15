#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, C2DVector&): forwards with a C2DVector(0, 0) default
struct C2DVector { float X; float Y; C2DVector(float x, float y) : X(x), Y(y) { } };
static const char* g_name; static C2DVector* g_value; static float g_dx = 1.0f, g_dy = 1.0f; static long g_calls;
struct CPersistContext {
    long pad[11];
    void Transfer(const char* name, C2DVector& value, const C2DVector& def) { g_name = name; g_value = &value; g_dx = def.X; g_dy = def.Y; ++g_calls; }
    void Transfer(const char* name, C2DVector& value) { Transfer(name, value, C2DVector(0.0f, 0.0f)); }
};
int main()
{
    CPersistContext c; C2DVector v(3.0f, 4.0f);
    c.Transfer("Offset", v);
    if (g_calls != 1 || std::strcmp(g_name, "Offset") != 0 || g_value != &v || g_dx != 0.0f || g_dy != 0.0f || v.X != 3.0f) { std::printf("00800d30_TEST FAIL\n"); return 1; }
    std::printf("00800d30_TEST PASS\n");
    return 0;
}
