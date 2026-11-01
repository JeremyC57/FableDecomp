// Test for CEngineBillboardGenerator::~CEngineBillboardGenerator (0x00B8C7C0): the pixel shader member drops its data
// reference before the vertex shader member (reverse member order), shader data is deleted only on its last reference,
// then the CEngineComponent base and the primary base run.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
struct ShaderData { unsigned long ReferenceCount; char tag; virtual ~ShaderData() { g_log[g_n++] = tag; } void Release() { if (--ReferenceCount == 0) delete this; } };
struct ShaderBase { virtual ~ShaderBase() { } };
struct Shader : ShaderBase { ShaderData* Data; ~Shader() { if (Data) Data->Release(); } };
struct RenderUtil { virtual ~RenderUtil() { g_log[g_n++] = 'U'; } };
struct EngineComponent { virtual ~EngineComponent() { g_log[g_n++] = 'C'; } void** g; bool r; unsigned char k; };
struct BillboardGenerator : RenderUtil, EngineComponent { Shader VertexShader; Shader PixelShader; };
int main()
{
    ShaderData* vs = new ShaderData; vs->ReferenceCount = 1; vs->tag = 'V';
    ShaderData* ps = new ShaderData; ps->ReferenceCount = 1; ps->tag = 'P';
    BillboardGenerator* g = new BillboardGenerator; g->VertexShader.Data = vs; g->PixelShader.Data = ps;
    delete g;
    if (g_n != 4 || g_log[0] != 'P' || g_log[1] != 'V' || g_log[2] != 'C' || g_log[3] != 'U') { std::printf("00b8c7c0_TEST FAIL\n"); return 1; }
    ShaderData* shared = new ShaderData; shared->ReferenceCount = 2; shared->tag = 'S';
    g = new BillboardGenerator; g->VertexShader.Data = shared; g->PixelShader.Data = 0;
    delete g;
    if (g_n != 6 || shared->ReferenceCount != 1) { std::printf("00b8c7c0_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("00b8c7c0_TEST PASS\n");
    return 0;
}