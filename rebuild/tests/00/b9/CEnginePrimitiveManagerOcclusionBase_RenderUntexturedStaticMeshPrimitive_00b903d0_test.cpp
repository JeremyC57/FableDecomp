// Behaviour test for 0x00B903D0 CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive: a copy of the model
// (recording device / managers) driven through the same logic; checks the shader + transform prefix, the PrepareForRender(1)
// flag dispatch and the per-block SetStreamSource / SetIndices / RealiseRenderState / DrawIndexedPrimitive sequence.
#include <cstdio>
#include <cstring>

enum EPrimitiveType { PT_NULL = -1, PT_TRIANGLE_LIST = 4, PT_TRIANGLE_STRIP = 5 };

static char g_log[4096];
static void Log(const char* fmt, long a = 0, long b = 0, long c = 0, long d = 0)
{
    char line[128];
    std::sprintf(line, fmt, a, b, c, d);
    std::strcat(g_log, line);
}

class CMatrix3x4 { public: float m[12]; };
class CVertexShader { public: long id; };
class CVertexBufferWin32 { public: long PVertexBuffer; unsigned char Stride; };
class CIndexBuffer { public: long PD3DIndexBuffer; EPrimitiveType PrimitiveType; };

struct CDevice {
    void SetStreamSource(unsigned long stream, long buffer, unsigned long offset, unsigned long stride) { Log("SS(%ld,%ld,%ld,%ld);", stream, buffer, offset, stride); }
    void SetIndices(long buffer) { Log("SI(%ld);", buffer); }
    void DrawIndexedPrimitive(long type, long base, unsigned long minIndex, unsigned long numVertices, unsigned long start, unsigned long count)
    {
        Log("DIP(%ld,%ld,", type, base);
        Log("%ld,%ld,%ld,%ld);", minIndex, numVertices, start, count);
    }
};
class CRenderManager { public: void RealiseRenderState() { Log("RRS;"); } };

class CShaderRenderManager { public:
    CRenderManager* RenderManager;
    CDevice* D3DDevice;
    bool WorldMatrixIsIdentity;
    bool FixedFunctionVertexMode;
    unsigned long UpdateFlags;
    void ApplyVertexShader(const CVertexShader& shader, bool force) { Log("AVS(%ld,%ld);", shader.id, force); }
    void SetWorldTransform(const CMatrix3x4& t) { Log("SWT(%ld);", (long)t.m[0]); }
    bool IsFixedFunctionVertexMode() const { return FixedFunctionVertexMode; }
    void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4) Log("VSC;");
        if (UpdateFlags & 1) Log("UPS;");
        if (UpdateFlags & 2) Log("UPSC;");
        if ((flags & 0x400) && (UpdateFlags & 0x20)) Log("BONE;");
        if (UpdateFlags & 4) Log("AMB;");
        if (UpdateFlags & 0x20000) Log("FOG;");
        if (UpdateFlags & 0x400) Log("LIGHT;");
        if (!(flags & 1) && !WorldMatrixIsIdentity) Log("RESETWORLD;");
        if (UpdateFlags & 0x80) Log("PROJ;");
        if ((flags & 4) && (UpdateFlags & 8)) Log("CAM;");
    }
    void SelectVertexBuffer(CVertexBufferWin32& buffer) { D3DDevice->SetStreamSource(0, buffer.PVertexBuffer, 0, buffer.Stride); }
    void DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer, unsigned long startIndex,
                            unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
        if (primitiveType == PT_NULL)
            primitiveType = indexBuffer.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
    }
    void DrawBuffersIndexed(CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        SelectVertexBuffer(vertexBuffer);
        DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
    }
};
struct CRenderManagerCore { void SetWorldTransform(const CMatrix3x4& t) { Log("CORE_SWT(%ld);", (long)t.m[0]); } };

static CShaderRenderManager* GPShaderRenderManager;
static CRenderManagerCore* g_pRenderManagerCore;

struct CStaticBlock {
    unsigned long PrimitiveCount, StartIndex;
    bool IsStrip;
    unsigned long GetPrimitiveCount() const { return PrimitiveCount; }
    unsigned long GetStartIndex() const { return StartIndex; }
    EPrimitiveType GetPrimitiveType() const { return IsStrip ? PT_TRIANGLE_STRIP : PT_TRIANGLE_LIST; }
};
class C3DPrimitive2 { public:
    CVertexBufferWin32* VertexBuffer;
    CIndexBuffer* IndexBuffer;
    unsigned long StaticBlockCount;
    CStaticBlock* StaticBlocks;
    unsigned long VertexCount;
    CStaticBlock* GetStaticBlock(unsigned long index) { return &StaticBlocks[index]; }
    unsigned long GetStaticBlockCount() const { return StaticBlockCount; }
    unsigned long GetVertexCount() const { return VertexCount; }
};

struct CEnginePrimitiveManagerOcclusionBase {
    CVertexShader VertexShader;
    void RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform)
    {
        GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
        if (GPShaderRenderManager->IsFixedFunctionVertexMode())
            g_pRenderManagerCore->SetWorldTransform(transform);
        else
            GPShaderRenderManager->SetWorldTransform(transform);
        GPShaderRenderManager->PrepareForRender(1);
        unsigned long block_count = primitive->GetStaticBlockCount();
        unsigned long vertex_count = primitive->GetVertexCount();
        for (unsigned long i = 0; i < block_count; ++i) {
            CStaticBlock* block = primitive->GetStaticBlock(i);
            GPShaderRenderManager->DrawBuffersIndexed(*primitive->VertexBuffer, 0, vertex_count, *primitive->IndexBuffer,
                                                      block->GetStartIndex(), block->GetPrimitiveCount(), block->GetPrimitiveType());
        }
    }
};

int main()
{
    CDevice device;
    CRenderManager renderManager;
    CShaderRenderManager manager = { &renderManager, &device, false, false, 0 };
    CRenderManagerCore core;
    GPShaderRenderManager = &manager;
    g_pRenderManagerCore = &core;

    CVertexBufferWin32 vertexBuffer = { 111, 24 };
    CIndexBuffer indexBuffer = { 222, PT_TRIANGLE_LIST };
    CStaticBlock blocks[3] = { { 10, 0, false }, { 7, 30, true }, { 3, 37, false } };
    C3DPrimitive2 primitive = { &vertexBuffer, &indexBuffer, 3, blocks, 500 };
    CMatrix3x4 transform = { { 9 } };
    CEnginePrimitiveManagerOcclusionBase occlusion = { { 42 } };
    bool ok = true;

    // shader path, nothing dirty: world transform through the shader manager (no reset: flag 1), three draws, list/strip/list
    g_log[0] = 0;
    occlusion.RenderUntexturedStaticMeshPrimitive(&primitive, transform);
    ok = ok && std::strcmp(g_log,
        "AVS(42,0);SWT(9);"
        "SS(0,111,0,24);SI(222);RRS;DIP(4,0,0,500,0,10);"
        "SS(0,111,0,24);SI(222);RRS;DIP(5,0,0,500,30,7);"
        "SS(0,111,0,24);SI(222);RRS;DIP(4,0,0,500,37,3);") == 0;

    // fixed-function path with every dirty bit this pass honours; bone (0x20) and camera (8) bits are not flushed by flags == 1
    g_log[0] = 0;
    manager.FixedFunctionVertexMode = true;
    manager.UpdateFlags = 4 | 1 | 2 | 0x20000 | 0x400 | 0x80 | 0x20 | 8;
    primitive.StaticBlockCount = 1;
    occlusion.RenderUntexturedStaticMeshPrimitive(&primitive, transform);
    ok = ok && std::strcmp(g_log,
        "AVS(42,0);CORE_SWT(9);VSC;UPS;UPSC;AMB;FOG;LIGHT;PROJ;"
        "SS(0,111,0,24);SI(222);RRS;DIP(4,0,0,500,0,10);") == 0;

    // no blocks: only the state prefix
    g_log[0] = 0;
    manager.UpdateFlags = 0;
    primitive.StaticBlockCount = 0;
    occlusion.RenderUntexturedStaticMeshPrimitive(&primitive, transform);
    ok = ok && std::strcmp(g_log, "AVS(42,0);CORE_SWT(9);") == 0;

    if (!ok) {
        std::printf("00b903d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b903d0_TEST PASS\n");
    return 0;
}