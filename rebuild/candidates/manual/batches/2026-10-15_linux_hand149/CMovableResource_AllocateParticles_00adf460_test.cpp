#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct C4DVector { float X, Y, Z, W; };
static C4DVector g_block[8];
struct Pool {
    bool ok; long lastSize; long handle;
    bool AllocateResource(long* h, void* owner, unsigned int size, unsigned int align)
    { lastSize = (long)size; if (!ok) return false; *h = 42; return true; }
    void* GetPointerFromHandle(long h) { handle = h; return g_block; }
};
struct Owner {
    C4DVector* m_pParticles; long m_memoryHandle; Pool* m_pMemoryPool;
    bool AllocateParticles(unsigned int count, Pool* pool)
    {
        m_pMemoryPool = pool;
        if ((u64)count * sizeof(C4DVector) > (u64)0xFFFFFFFFu) return false;
        if (!pool->AllocateResource(&m_memoryHandle, this, count * (unsigned int)sizeof(C4DVector), 0)) return false;
        m_pParticles = (C4DVector*)m_pMemoryPool->GetPointerFromHandle(m_memoryHandle);
        return true;
    }
};
int main()
{
    Pool p; p.ok = true; p.lastSize = 0; p.handle = 0;
    Owner o; o.m_pParticles = 0; o.m_memoryHandle = 0; o.m_pMemoryPool = 0;
    bool fail = false;
    if (!o.AllocateParticles(8, &p) || o.m_pParticles != g_block || p.lastSize != 128 || p.handle != 42 || o.m_pMemoryPool != &p) fail = true;
    if (o.AllocateParticles(0x10000000u, &p)) fail = true;
    p.ok = false;
    if (o.AllocateParticles(2, &p)) fail = true;
    if (fail) { std::printf("00adf460_TEST FAIL\n"); return 1; }
    std::printf("00adf460_TEST PASS\n");
    return 0;
}
