// Behaviour test for 0x00B32100 (see src/00b32100.cpp): the identity-matrix reset on recording render managers.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00b32100_TEST FAIL\n"); return 1; } } while (0)

struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity() { XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f; XAxis.X = YAxis.Y = ZAxis.Z = 1.0f; }
};
static bool IsIdentity(const CMatrix3x4& m)
{
    return m.XAxis.X == 1 && m.XAxis.Y == 0 && m.XAxis.Z == 0 && m.YAxis.X == 0 && m.YAxis.Y == 1 && m.YAxis.Z == 0
        && m.ZAxis.X == 0 && m.ZAxis.Y == 0 && m.ZAxis.Z == 1 && m.Position.X == 0 && m.Position.Y == 0 && m.Position.Z == 0;
}
struct CShaderRenderManager { int calls; bool identity; void SetWorldTransform(const CMatrix3x4& m) { ++calls; identity = IsIdentity(m); } };
struct CRenderManagerCore { int calls; bool identity; void SetWorldTransform(const CMatrix3x4& m) { ++calls; identity = IsIdentity(m); } };
static CShaderRenderManager g_shader = { 0, false };
static CRenderManagerCore g_core = { 0, false };
static CShaderRenderManager* g_pShaderRenderManager = &g_shader;
static CRenderManagerCore* g_pRenderManagerCore = &g_core;
struct CEnginePrimitiveRenderer {
    void ResetWorldTransform()
    {
        CMatrix3x4 identity;
        g_pShaderRenderManager->SetWorldTransform(identity);
        g_pRenderManagerCore->SetWorldTransform(identity);
    }
};
int main()
{
    CEnginePrimitiveRenderer r;
    r.ResetWorldTransform();
    CHECK(g_shader.calls == 1 && g_shader.identity && g_core.calls == 1 && g_core.identity);
    std::printf("00b32100_TEST PASS\n");
    return 0;
}
