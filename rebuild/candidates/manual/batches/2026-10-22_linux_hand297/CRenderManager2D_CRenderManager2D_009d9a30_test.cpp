#include <cstdio>
#include <cstring>

// Model of the CRenderManager2D constructor: the quick-draw queues start empty; in retail-bank mode the retail shader
// bank <dataPath>pc\shaders.big is opened first; when the SHADERS_BBBLIB bank exists it is opened read-only (flags
// 0x48), a vertex shader resource is created over it with the 2D declaration and format 0x1c4, and VSHADER_BBBLIB_2D
// becomes the 2D vertex shader (reference counted); flag bit 0 enables the alternate render-target clear.
struct CShaderData { int refs; char name[32]; };
static bool g_retailMode; static char g_openedRetail[64]; static int g_retailOpens;
static bool g_bankExists; static char g_bankOpened[32]; static unsigned long g_bankFlags;
static CShaderData g_shader;

struct CResource {
    const void* bank; int declaration; int format;
    CShaderData* GetVertexShader(const char* name) { std::strcpy(g_shader.name, name); return &g_shader; }
};
static CResource g_resource;

struct CRenderManager2D {
    char bank;   // stands in for VertexShaderBank
    CResource* VertexShaderResource;
    CShaderData* VertexShader2D;
    int queueSizes[6];
    bool AlternateRenderTargetClearEnabled;
    CRenderManager2D(const char* dataPath, unsigned long flags) : VertexShaderResource(0), VertexShader2D(0)
    {
        for (int i = 0; i < 6; ++i) queueSizes[i] = 0;
        if (g_retailMode) {
            std::strcpy(g_openedRetail, dataPath); std::strcat(g_openedRetail, "pc\\"); std::strcat(g_openedRetail, "shaders.big");
            ++g_retailOpens;
        }
        if (g_bankExists) {
            std::strcpy(g_bankOpened, "SHADERS_BBBLIB"); g_bankFlags = 0x48;
            VertexShaderResource = &g_resource;
            VertexShaderResource->bank = &bank;
            VertexShaderResource->declaration = 2;
            VertexShaderResource->format = 0x1c4;
            CShaderData* shader = VertexShaderResource->GetVertexShader("VSHADER_BBBLIB_2D");
            if (VertexShader2D) --VertexShader2D->refs;
            VertexShader2D = shader;
            ++VertexShader2D->refs;
        }
        AlternateRenderTargetClearEnabled = (flags & 1) != 0;
    }
};

static void Reset() { g_openedRetail[0] = 0; g_retailOpens = 0; g_bankOpened[0] = 0; g_bankFlags = 0; g_shader.refs = 0; g_shader.name[0] = 0; }

int main()
{
    Reset(); g_retailMode = true; g_bankExists = true;
    CRenderManager2D a("data\\", 3);
    if (g_retailOpens != 1 || std::strcmp(g_openedRetail, "data\\pc\\shaders.big") != 0 || std::strcmp(g_bankOpened, "SHADERS_BBBLIB") != 0
        || g_bankFlags != 0x48 || a.VertexShaderResource != &g_resource || g_resource.bank != &a.bank || g_resource.format != 0x1c4
        || a.VertexShader2D != &g_shader || g_shader.refs != 1 || std::strcmp(g_shader.name, "VSHADER_BBBLIB_2D") != 0
        || !a.AlternateRenderTargetClearEnabled || a.queueSizes[5] != 0)
        { std::printf("009d9a30_TEST FAIL\n"); return 1; }
    Reset(); g_retailMode = false; g_bankExists = false;
    CRenderManager2D b("x", 2);
    if (g_retailOpens != 0 || g_bankOpened[0] != 0 || b.VertexShaderResource != 0 || b.VertexShader2D != 0 || b.AlternateRenderTargetClearEnabled)
        { std::printf("009d9a30_TEST FAIL\n"); return 1; }
    std::printf("009d9a30_TEST PASS\n");
    return 0;
}
