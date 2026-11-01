// Test for CEngineSkyRenderer::~CEngineSkyRenderer() -- the body uninitialises the vertex buffer then the index buffer; the members are then destroyed in reverse order (the def pointer, the pixel shader) and the CEngineComponent base destructor runs last.
#include <cstdio>
#include <cstring>
static char g_trace[64]; static int g_n = 0;
static void mark(char c) { g_trace[g_n++] = c; g_trace[g_n] = 0; }
class CEngineComponent { public: virtual ~CEngineComponent() { mark('B'); } };
class CPixelShader { public: ~CPixelShader() { mark('P'); } };
class CVertexBufferWin32 { public: bool live; void Uninitialise() { if (live) mark('V'); live = false; } ~CVertexBufferWin32() { Uninitialise(); } };
class CIndexBuffer { public: bool live; void Uninitialise() { if (live) mark('I'); live = false; } ~CIndexBuffer() { Uninitialise(); } };
struct CDefPointer { ~CDefPointer() { mark('D'); } };
class CEngineSkyRenderer : public CEngineComponent { public:
    CPixelShader PixelShader;
    CVertexBufferWin32 VertexBuffer;
    CIndexBuffer IndexBuffer;
    CDefPointer PSkyDef;
    virtual ~CEngineSkyRenderer() { VertexBuffer.Uninitialise(); IndexBuffer.Uninitialise(); }
};
int main()
{
    CEngineSkyRenderer* r = new CEngineSkyRenderer;
    r->VertexBuffer.live = true; r->IndexBuffer.live = true;
    CEngineComponent* c = r;
    delete c;
    if (std::strcmp(g_trace, "VIDPB") != 0) { std::printf("00b621a0_TEST FAIL\n"); return 1; }
    std::printf("00b621a0_TEST PASS\n");
    return 0;
}