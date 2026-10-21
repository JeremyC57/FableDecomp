// behaviour test for 00bb99a0 ~CEnginePrimitiveManagerParticleDecalGroup: shaders release their shared data (deleted at refcount 0),
// buffers uninitialise, members go in reverse declaration order
#include <cstdio>
static int g_deleted = 0;
static int g_order[32];
static int g_n = 0;
struct CShaderData { long refCount; virtual ~CShaderData() { ++g_deleted; } };
struct CShader {
    CShaderData* Data; int tag;
    CShader() : Data(0), tag(0) {}
    ~CShader() { g_order[g_n++] = tag; if (Data) { if (--Data->refCount == 0) delete Data; } }
};
struct CBuffer { int tag; CBuffer() : tag(0) {} ~CBuffer() { g_order[g_n++] = tag; } };
struct CDecalGroup {
    unsigned long PointLightShaderIndex[6];
    CShader LandscapeVS[3], MeshVS[3], TwoPassVS, OnePassVS, ClearVS;
    CShader LandscapePS, MeshPS, TwoPassPS, OnePassPS, ClearPS;
    CBuffer LandscapeVB, LandscapeIB, MeshVB, MeshIB;
};
int main()
{
    CDecalGroup* g = new CDecalGroup;
    CShaderData* shared = new CShaderData; shared->refCount = 2;
    CShaderData* single = new CShaderData; single->refCount = 1;
    g->MeshVS[0].Data = shared; g->ClearPS.Data = shared; g->LandscapeVS[2].Data = single;
    g->MeshIB.tag = 1; g->LandscapeVS[0].tag = 2; g->LandscapeVS[2].tag = 3;
    delete g;
    if (g_deleted != 2 || g_n != 18) { std::printf("00bb99a0_TEST FAIL\n"); return 1; }
    if (g_order[0] != 1 || g_order[17] != 2 || g_order[15] != 3) { std::printf("00bb99a0_TEST FAIL\n"); return 1; }
    std::printf("00bb99a0_TEST PASS\n");
    return 0;
}
