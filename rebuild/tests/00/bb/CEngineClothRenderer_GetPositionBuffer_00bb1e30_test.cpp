// behaviour test for 0x00BB1E30 CEngineClothRenderer::GetPositionBuffer(unsigned long size)
#include <cstdio>
#include <cstdlib>

class C4DVector { public: float X, Y, Z, W; };

struct Pool {
    int FreeAllCalls;
    unsigned int LastBytes;
    void* Blocks[8];
    int NumBlocks;
    Pool() : FreeAllCalls(0), LastBytes(0), NumBlocks(0) { }
    void FreeAll() { ++FreeAllCalls; for (int i = 0; i < NumBlocks; ++i) std::free(Blocks[i]); NumBlocks = 0; }
    void* Allocate(unsigned int bytes) { LastBytes = bytes; void* p = std::malloc(bytes); Blocks[NumBlocks++] = p; return p; }
};

template <class T> T* PoolNewArray(Pool& pool, unsigned int count)
{
    return count != 0 ? (count > 0xffffffffu / (unsigned int)sizeof(T) ? 0 : (T*)pool.Allocate(count * (unsigned int)sizeof(T))) : 0;
}

struct Renderer {
    Pool BufferPool;
    C4DVector* Buffer;
    unsigned int BufferSize;
    Renderer() : Buffer(0), BufferSize(0) { }
    C4DVector* GetBuffer(unsigned int size)
    {
        if (size > BufferSize) {
            BufferPool.FreeAll();
            Buffer = PoolNewArray<C4DVector>(BufferPool, size);
            BufferSize = size;
        }
        return Buffer;
    }
};

int main()
{
    Renderer r;
    if (r.GetBuffer(0) != 0 || r.BufferPool.FreeAllCalls != 0) { std::printf("00bb1e30_TEST FAIL\n"); return 1; }
    C4DVector* a = r.GetBuffer(10);
    if (a == 0 || r.BufferSize != 10 || r.BufferPool.LastBytes != 160 || r.BufferPool.FreeAllCalls != 1) { std::printf("00bb1e30_TEST FAIL\n"); return 1; }
    if (r.GetBuffer(4) != a || r.BufferPool.FreeAllCalls != 1) { std::printf("00bb1e30_TEST FAIL\n"); return 1; }
    C4DVector* b = r.GetBuffer(20);
    if (b == 0 || r.BufferSize != 20 || r.BufferPool.LastBytes != 320 || r.BufferPool.FreeAllCalls != 2) { std::printf("00bb1e30_TEST FAIL\n"); return 1; }
    r.BufferPool.FreeAll();
    std::printf("00bb1e30_TEST PASS\n");
    return 0;
}