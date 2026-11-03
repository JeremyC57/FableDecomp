// Behaviour test for 0x00B71880 ~CEngineWaterRenderer: the body releases the three constant buffers and frees the dynamic texture
// page (handle cleared), then the members are destroyed: shared shader data loses one reference (deleted at zero), the render
// lists free their storage, and the counted pointers release their blocks.
#include <cstdio>
#include <cstdlib>
static int gUninit = 0, gFreedPage = -1, gShaderDataDeleted = 0, gBlocksDeleted = 0, gBaseDtor = 0;
struct CIndexBuffer { int live; CIndexBuffer() : live(1) { } void Uninitialise() { if (live) { live = 0; ++gUninit; } } ~CIndexBuffer() { Uninitialise(); } };
struct CVertexBufferWin32 { int live; CVertexBufferWin32() : live(1) { } void Uninitialise() { if (live) { live = 0; ++gUninit; } } ~CVertexBufferWin32() { Uninitialise(); } };
struct CShaderData { unsigned long ReferenceCount; virtual ~CShaderData() { ++gShaderDataDeleted; } };
struct CShader { CShaderData* Data; CShader() : Data(0) { } ~CShader() { CShaderData* d = Data; if (d) { if (--d->ReferenceCount == 0) delete d; } } };
struct CInfo { unsigned long RefCount; };
struct CCounted { CInfo* Info; CCounted() : Info(0) { } ~CCounted() { if (Info) { if (--Info->RefCount == 0) { ++gBlocksDeleted; delete Info; } } Info = 0; } };
struct CList { void* Start; CList() : Start(malloc(16)) { } ~CList() { if (Start) free(Start); } };
struct CGPUScratchBufferTextureManager { void FreeMemoryPage(long page) { gFreedPage = (int)page; } };
static CGPUScratchBufferTextureManager gScratch;
struct CEngineComponent { virtual ~CEngineComponent() { ++gBaseDtor; } };
class CEngineWaterRenderer : public CEngineComponent { public:
    CIndexBuffer ForegroundIndexBuffers, ForegroundTextureIndexBuffers;
    CVertexBufferWin32 ForegroundTextureVertexBuffers;
    CIndexBuffer SeaBackgroundIndexBuffer;
    CVertexBufferWin32 SeaBackgroundVertexBuffer;
    CShader SeaBackgroundPixelShader, IceBackgroundVertexShader;
    long DynamicTexturePageHandle;
    CList ForegroundRenderList;
    CCounted LoadingSeaEntry, LoadingSea;
    void DeleteConstantBuffers() { ForegroundIndexBuffers.Uninitialise(); ForegroundTextureIndexBuffers.Uninitialise(); ForegroundTextureVertexBuffers.Uninitialise(); }
    void UninitialiseDynamicTextures() { gScratch.FreeMemoryPage(DynamicTexturePageHandle); DynamicTexturePageHandle = 0; }
    virtual ~CEngineWaterRenderer() { DeleteConstantBuffers(); if (gUninit != 3) gUninit = -100; UninitialiseDynamicTextures(); }
};
int main()
{
    CEngineWaterRenderer* r = new CEngineWaterRenderer;
    r->DynamicTexturePageHandle = 7;
    CShaderData* a = new CShaderData; a->ReferenceCount = 1;
    CShaderData* b = new CShaderData; b->ReferenceCount = 2;
    r->SeaBackgroundPixelShader.Data = a;
    r->IceBackgroundVertexShader.Data = b;
    r->LoadingSea.Info = new CInfo; r->LoadingSea.Info->RefCount = 1;
    CEngineComponent* c = r;
    delete c;
    if (gUninit != 5 || gFreedPage != 7 || gShaderDataDeleted != 1 || b->ReferenceCount != 1 || gBlocksDeleted != 1 || gBaseDtor != 1) {
        std::printf("00b71880_TEST FAIL\n");
        return 1;
    }
    std::printf("00b71880_TEST PASS\n");
    return 0;
}
