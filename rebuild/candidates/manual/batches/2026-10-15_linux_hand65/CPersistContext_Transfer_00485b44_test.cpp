#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, C3DVector&): forwards with a C3DVector(0, 0, 0) default
struct C3DVector { float X; float Y; float Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static const char* g_name; static C3DVector* g_value; static float g_d[3] = { 1.0f, 1.0f, 1.0f }; static long g_calls;
struct CPersistContext {
    long pad[11];
    void Transfer(const char* name, C3DVector& value, const C3DVector& def) { g_name = name; g_value = &value; g_d[0] = def.X; g_d[1] = def.Y; g_d[2] = def.Z; ++g_calls; }
    void Transfer(const char* name, C3DVector& value) { Transfer(name, value, C3DVector(0.0f, 0.0f, 0.0f)); }
};
int main()
{
    CPersistContext c; C3DVector v(1.0f, 2.0f, 3.0f);
    c.Transfer("Position", v);
    if (g_calls != 1 || std::strcmp(g_name, "Position") != 0 || g_value != &v || g_d[0] != 0.0f || g_d[1] != 0.0f || g_d[2] != 0.0f || v.Z != 3.0f) { std::printf("00485b44_TEST FAIL\n"); return 1; }
    std::printf("00485b44_TEST PASS\n");
    return 0;
}
