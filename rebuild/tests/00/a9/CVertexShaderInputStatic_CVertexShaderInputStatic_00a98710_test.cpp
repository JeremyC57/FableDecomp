#include <cstdio>
static int g_base = 0; static unsigned long g_flags = 0; static int g_dyn = -1; static int g_order = 0;
class CVertexShaderInput { public: virtual void v00() { } CVertexShaderInput() { ++g_base; g_order = 1; } };
struct CVertexShaderInputStatic : CVertexShaderInput {
    virtual void v00() { }
    CVertexShaderInputStatic(unsigned long flags, bool dynamic) : CVertexShaderInput() { Initialise(flags, dynamic); }
    void Initialise(unsigned long flags, bool dynamic) { g_flags = flags; g_dyn = dynamic ? 1 : 0; g_order = g_order * 10 + 2; }
};
int main()
{
    CVertexShaderInputStatic a(0x1234, true);
    if (g_base != 1 || g_flags != 0x1234 || g_dyn != 1 || g_order != 12) { std::printf("00a98710_TEST FAIL\n"); return 1; }
    CVertexShaderInputStatic b(7, false);
    if (g_base != 2 || g_flags != 7 || g_dyn != 0) { std::printf("00a98710_TEST FAIL\n"); return 1; }
    std::printf("00a98710_TEST PASS\n"); return 0;
}