#include <cstdio>
#include <cstdlib>
static int g_disposal = 0; static int g_inits = 0;
static void* AllocFromGlobalAllocator(unsigned int size) { return std::malloc(size); }
struct CVertexCompressionParams {
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams() : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f), PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase {
    void* PVertexBuffer; unsigned int Length; long Type; unsigned char Stride; CVertexCompressionParams CompressionParams;
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
    virtual ~CVertexBufferBase() { }
};
struct CVertexBuffer : CVertexBufferBase {
    long InitType, InitCount, InitStride, InitPool; unsigned int InitUsage;
    CVertexBuffer() { PVertexBuffer = 0; Stride = 0; }
    bool Initialise(long type, long count, long stride, unsigned int usage, long pool)
    { ++g_inits; InitType = type; InitCount = count; InitStride = stride; InitUsage = usage; InitPool = pool; return true; }
    static void* operator new(size_t size) { return AllocFromGlobalAllocator((unsigned int)size); }
    static void operator delete(void* p) { std::free(p); }
};
struct CManager {
    void ProcessDisposalLists() { ++g_disposal; }
    CVertexBuffer* AllocateVertexBuffer(long, long type, long count, long stride, unsigned int usage)
    {
        ProcessDisposalLists();
        CVertexBuffer* buffer = new CVertexBuffer;
        buffer->Initialise(type, count, stride, usage, 1);
        return buffer;
    }
};
int main()
{
    CManager m;
    CVertexBuffer* b = m.AllocateVertexBuffer(0, 3, 64, 24, 0x10);
    if (!b || g_disposal != 1 || g_inits != 1 || b->PVertexBuffer != 0 || b->Length != 0 || b->Stride != 0 || b->CompressionParams.PosScaleW != 1.0f
        || b->CompressionParams.PosBiasX != 0.0f || b->InitType != 3 || b->InitCount != 64 || b->InitStride != 24 || b->InitUsage != 0x10 || b->InitPool != 1) {
        std::printf("00bda3d0_TEST FAIL\n"); return 1;
    }
    delete b;
    std::printf("00bda3d0_TEST PASS\n");
    return 0;
}
