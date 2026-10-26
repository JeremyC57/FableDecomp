#include <cstdio>
#include <cstdlib>
static int g_dataDeleted = 0, g_ibUninit = 0, g_vbUninit = 0, g_rootDtor = 0, g_freed = 0, g_pageFreed = -1, g_restore = 0, g_surfUninit = 0, g_baseClassDtor = 0, g_rangeDestroyed = 0;
struct CPixelShaderData { virtual ~CPixelShaderData() { ++g_dataDeleted; } long m_refCount; CPixelShaderData(long r) : m_refCount(r) {} };
struct CVertexShaderData { virtual ~CVertexShaderData() { ++g_dataDeleted; } long m_refCount; CVertexShaderData(long r) : m_refCount(r) {} };
class CShaderBase { public: virtual ~CShaderBase() {} };
class CPixelShader : public CShaderBase { public:
    CPixelShaderData* Data;
    CPixelShader() : Data(0) {}
    virtual ~CPixelShader() { CPixelShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
class CVertexShader : public CShaderBase { public:
    CVertexShaderData* Data;
    CVertexShader() : Data(0) {}
    void Release() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } Data = 0; }
    virtual ~CVertexShader() { CVertexShaderData* data = Data; if (data) { if (--data->m_refCount == 0) delete data; } }
};
static void my_free(void* p) { ++g_freed; std::free(p); }

struct CRenderStateCaptureBlock { bool Open; CRenderStateCaptureBlock() : Open(false) {} ~CRenderStateCaptureBlock() { if (Open) { Open = false; ++g_restore; } } };
class CBaseClass { public: ~CBaseClass() { ++g_baseClassDtor; } };
class CSurface { public: void Uninitialise() { ++g_surfUninit; } ~CSurface() { Uninitialise(); } };
class CRenderTarget : public CBaseClass { public: CSurface RenderToSurface; CSurface ZBufferSurface; };
class CEngineComponent { public: ~CEngineComponent() { ++g_rootDtor; } };
class CEngineScreenEffectOutlineGlow : public CEngineComponent { public:
    long DynamicTexturePageHandle;
    CRenderTarget OldRenderTarget;
    CVertexShader OutlineToBlurVertexShader; CPixelShader OutlineToBlurPixelShader;
    CVertexShader BlurToBlurVertexShader; CPixelShader BlurToBlurPixelShader;
    CVertexShader BlurToBackBufferVertexShader; CPixelShader BlurToBackBufferPixelShader;
    CRenderStateCaptureBlock OutlineRenderingCaptureBlock;
    ~CEngineScreenEffectOutlineGlow();
};
CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow() { g_pageFreed = DynamicTexturePageHandle; }
int main()
{
    CEngineScreenEffectOutlineGlow* g = new CEngineScreenEffectOutlineGlow;
    g->DynamicTexturePageHandle = 7;
    g->OutlineRenderingCaptureBlock.Open = true;
    g->OutlineToBlurVertexShader.Data = new CVertexShaderData(1);
    g->BlurToBackBufferPixelShader.Data = new CPixelShaderData(1);
    CPixelShaderData* shared = new CPixelShaderData(2);
    g->BlurToBlurPixelShader.Data = shared;
    delete g;
    bool ok = g_pageFreed == 7 && g_restore == 1 && g_dataDeleted == 2 && shared->m_refCount == 1
           && g_surfUninit == 2 && g_baseClassDtor == 1 && g_rootDtor == 1;
    delete shared;
    if (!ok) { std::printf("00b898d0_TEST FAIL\n"); return 1; }
    std::printf("00b898d0_TEST PASS\n");
    return 0;
}