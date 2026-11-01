// Test for CRenderManager2D::~CRenderManager2D (0x009C2FD0): members destroyed in reverse declaration order (tri-info /
// vertex vector pairs from the last down, the vertex shader releasing its shared data at zero, the shader-resource counted
// pointer, the shader bank) and then the CRenderManagerCore base.
#include <cstdio>
#include <cstring>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct Core { virtual ~Core() { Log('C'); } };
struct Bank { ~Bank() { Log('B'); } };
struct Counted { int* Info; ~Counted() { if (Info) { if (--*Info == 0) Log('r'); } Log('P'); } };
struct ShaderData { int RefCount; virtual ~ShaderData() { Log('d'); } };
struct VertexShader { ShaderData* Data; ~VertexShader() { ShaderData* d = Data; if (d) { if (--d->RefCount == 0) delete d; } Log('S'); } };
struct Verts { char tag; ~Verts() { Log(tag); } };
struct Tris { char tag; ~Tris() { Log(tag); } };
struct RM2D : Core {
    Bank VertexShaderBank; Counted VertexShaderResource; VertexShader VertexShader2D;
    Verts v1; Tris t1; Verts v2; Tris t2; Verts v3; Tris t3;
};
int main()
{
    {
        RM2D* r = new RM2D;
        int count = 1; r->VertexShaderResource.Info = &count;
        r->VertexShader2D.Data = new ShaderData; r->VertexShader2D.Data->RefCount = 1;
        r->v1.tag = '1'; r->t1.tag = 'a'; r->v2.tag = '2'; r->t2.tag = 'b'; r->v3.tag = '3'; r->t3.tag = 'c';
        Core* base = r;
        delete base;
    }
    if (std::strcmp(g_log, "c3b2a1dSrPBC") != 0) { std::printf("009c2fd0_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    {
        ShaderData* shared = new ShaderData; shared->RefCount = 2;
        RM2D* r = new RM2D; r->VertexShaderResource.Info = 0; r->VertexShader2D.Data = shared;
        r->v1.tag = r->t1.tag = r->v2.tag = r->t2.tag = r->v3.tag = r->t3.tag = '.';
        delete r;
        if (shared->RefCount != 1) { std::printf("009c2fd0_TEST FAIL\n"); return 1; }
        delete shared;
    }
    std::printf("009c2fd0_TEST PASS\n");
    return 0;
}