// Behaviour test for CEnginePrimitiveManagerRepeatedStaticMeshes::~CEnginePrimitiveManagerRepeatedStaticMeshes: free the SIMD
// variable block first, then release the twelve pixel/vertex shaders in reverse declaration order (shared data deleted only
// when its count reaches zero), then the occlusion-base destructor.
#include <cstdio>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct CShaderData {
    int  Id;
    long ReferenceCount;
    virtual ~CShaderData() { Log(1000 + Id); }
};
class CShaderBase { public: virtual ~CShaderBase() { } };
class CPixelShader : public CShaderBase { public:
    CShaderData* Data;
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader() { if (Data) { CShaderData* current = Data; if (--current->ReferenceCount == 0) delete current; } }
};
class CVertexShader : public CShaderBase { public:
    CShaderData* Data;
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader() { if (Data) { CShaderData* current = Data; if (--current->ReferenceCount == 0) delete current; } }
};
struct CSIMDVariableBlock { int Id; };
static void DeallocateAligned(void* p) { Log(p ? 500 + ((CSIMDVariableBlock*)p)->Id : 499); }   // CMemoryAllocator::DeallocateAligned
class CEnginePrimitiveManagerOcclusionBase { public:
    virtual ~CEnginePrimitiveManagerOcclusionBase() { Log(9000); }
};
class CEnginePrimitiveManagerRepeatedStaticMeshes : public CEnginePrimitiveManagerOcclusionBase { public:
    CPixelShader  PixelShader;
    CVertexShader VertexShader;
    CPixelShader  PixelShaderStippleAlpha;
    CVertexShader VertexShaderStippleAlpha;
    CPixelShader  PixelShaderDepthBufferShadowed;
    CVertexShader VertexShaderDepthBufferShadowed;
    CPixelShader  PixelShaderDepthBufferShadowedStippleAlpha;
    CVertexShader VertexShaderDepthBufferShadowedStippleAlpha;
    CPixelShader  PixelShaderColourBufferShadowed;
    CVertexShader VertexShaderColourBufferShadowed;
    CPixelShader  PixelShaderColourBufferShadowedStippleAlpha;
    CVertexShader VertexShaderColourBufferShadowedStippleAlpha;
    CPixelShader* PixelShaderShadowed;
    CVertexShader* VertexShaderShadowed;
    CPixelShader* PixelShaderShadowedStippleAlpha;
    CVertexShader* VertexShaderShadowedStippleAlpha;
    CSIMDVariableBlock* SIMDVariableBlock;
    virtual ~CEnginePrimitiveManagerRepeatedStaticMeshes() { DeallocateAligned(SIMDVariableBlock); }
};
static CShaderData* Make(int id, long refs) { CShaderData* d = new CShaderData; d->Id = id; d->ReferenceCount = refs; return d; }
int main()
{
    CEnginePrimitiveManagerRepeatedStaticMeshes* m = new CEnginePrimitiveManagerRepeatedStaticMeshes;
    CSIMDVariableBlock block; block.Id = 7;
    m->SIMDVariableBlock = &block;
    m->PixelShader.Data = Make(1, 1);
    m->VertexShader.Data = Make(2, 1);
    CShaderData* shared = Make(3, 2);                       // shared by two members: deleted by the second release
    m->PixelShaderStippleAlpha.Data = shared;
    m->PixelShaderColourBufferShadowed.Data = shared;
    m->VertexShaderColourBufferShadowedStippleAlpha.Data = Make(12, 1);
    m->PixelShaderShadowed = 0; m->VertexShaderShadowed = 0; m->PixelShaderShadowedStippleAlpha = 0; m->VertexShaderShadowedStippleAlpha = 0;
    CEnginePrimitiveManagerOcclusionBase* base = m;
    delete base;
    const int want[] = { 507, 1012, 1003, 1002, 1001, 9000 };
    int n = sizeof(want) / sizeof(want[0]);
    bool ok = g_n == n;
    for (int i = 0; ok && i < n; ++i) ok = g_log[i] == want[i];
    if (!ok) { std::printf("00b999f0_TEST FAIL\n"); return 1; }
    std::printf("00b999f0_TEST PASS\n");
    return 0;
}