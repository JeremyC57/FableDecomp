// Behaviour test for 0x00B99670 (see src/00b99670.cpp): UploadShaderConstants on a recording shader manager.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b99670_TEST FAIL\n"); return 1; } } while (0)

class C4DVector { public: float X, Y, Z, W; };
class CShaderRenderManager { public:
    int userCalls; unsigned int idx[2]; float v[2][4]; int vsCalls; int regs[2]; const void* data[2]; unsigned int counts[2];
    void SetUserVertexShaderConstant(unsigned int i, float x, float y, float z, float w)
    { idx[userCalls] = i; v[userCalls][0] = x; v[userCalls][1] = y; v[userCalls][2] = z; v[userCalls][3] = w; ++userCalls; }
    void SetVSConstants(int reg, const void* d, unsigned int n) { regs[vsCalls] = reg; data[vsCalls] = d; counts[vsCalls] = n; ++vsCalls; }
};
static CShaderRenderManager g_shader;
static float g_fadeScale = 4.0f;
static int g_matrixRegister = 30, g_offsetRegister = 40;
struct CPrim { float AlphaTransformMult; float AlphaTransformOffset; };
static void Upload(CPrim& p, const C4DVector* offsets, const C4DVector* matrices, int count)
{
    g_shader.SetUserVertexShaderConstant(1, p.AlphaTransformMult / g_fadeScale, 0.0f, 0.0f, p.AlphaTransformOffset);
    g_shader.SetUserVertexShaderConstant(2, 1.0f, -1.0f, 1.0f, 0.0f);
    g_shader.SetVSConstants(g_matrixRegister, matrices, count);
    g_shader.SetVSConstants(g_offsetRegister, offsets, count);
}
int main()
{
    std::memset(&g_shader, 0, sizeof(g_shader));
    CPrim p = { 2.0f, 0.5f }; C4DVector a[2], b[2];
    Upload(p, a, b, 2);
    CHECK(g_shader.userCalls == 2 && g_shader.idx[0] == 1 && g_shader.v[0][0] == 0.5f && g_shader.v[0][3] == 0.5f);
    CHECK(g_shader.idx[1] == 2 && g_shader.v[1][0] == 1 && g_shader.v[1][1] == -1 && g_shader.v[1][2] == 1 && g_shader.v[1][3] == 0);
    CHECK(g_shader.vsCalls == 2 && g_shader.regs[0] == 30 && g_shader.data[0] == b && g_shader.regs[1] == 40 && g_shader.data[1] == a);
    CHECK(g_shader.counts[0] == 2 && g_shader.counts[1] == 2);
    std::printf("00b99670_TEST PASS\n");
    return 0;
}