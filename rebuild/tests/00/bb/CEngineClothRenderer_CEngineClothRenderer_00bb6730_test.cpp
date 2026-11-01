// Test for CEngineClothRenderer::~CEngineClothRenderer() -- the implicit destructor: members go in reverse order (the two memory pools clear their blocks, the index buffer uninitialises, the vertex shader drops its shared data and deletes it on the last reference, the arrays free their storage), then the CBaseClassNonCopyable base.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_trace[64]; static int g_n = 0;
static void mark(char c) { g_trace[g_n++] = c; g_trace[g_n] = 0; }
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { mark('B'); } };
struct CArray { void* data; ~CArray() { if (data) { std::free(data); mark('A'); } } };
struct CVertexShaderData { int refs; bool RemoveReference() { return --refs != 0; } ~CVertexShaderData() { mark('d'); } };
class CVertexShader { public: CVertexShaderData* Data; ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } mark('S'); } };
class CIndexBuffer { public: ~CIndexBuffer() { mark('I'); } };
class CSimpleMemoryPool { public: char id; ~CSimpleMemoryPool() { mark(id); } };
class CEngineClothRenderer : public CBaseClassNonCopyable { public:
    CArray Vertices;
    CVertexShader Shader;
    CIndexBuffer IndexBuffer;
    CSimpleMemoryPool PositionBufferPool;
    CSimpleMemoryPool NormalBufferPool;
};
int main()
{
    CEngineClothRenderer* r = new CEngineClothRenderer;
    r->Vertices.data = std::malloc(16);
    r->Shader.Data = new CVertexShaderData; r->Shader.Data->refs = 1;
    r->PositionBufferPool.id = 'p'; r->NormalBufferPool.id = 'n';
    CBaseClassNonCopyable* b = r;
    delete b;
    if (std::strcmp(g_trace, "npIdSAB") != 0) { std::printf("00bb6730_TEST FAIL\n"); return 1; }
    std::printf("00bb6730_TEST PASS\n");
    return 0;
}