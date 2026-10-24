// CEngineClothRenderer::GetPositionBuffer(unsigned long size) -- return the cloth position buffer, regrowing it from its frame pool (old contents dropped) when more than the current size is asked for
// Leaf: GetPositionBuffer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1E30, 93 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb ?GetPositionBuffer@CEngineClothRenderer@@QAEPAVC4DVector@@K@Z; callers are the cloth-simulation
//   Render bodies 0x00B813CF / 0x00B81580 / 0x00B81710. Catalog "CSimpleMemoryPool_ReallocateBuffer" is descriptive.
// Callees: CSimpleMemoryPool::FreeAll 0x00A5C720, CSimpleMemoryPool::Allocate(unsigned long) 0x00A5C910 (both landed).
// Layout: Ego_r.pdb CEngineClothRenderer less 0x18 (six 12-byte retail CArrays, PDB 16): PositionBufferPool +0xA8,
//   PositionBuffer +0xC4, PositionBufferSize +0xC8. There is no engine header for the class; the pool needs its member
//   functions, so both are local models (as the landed pool callers 0x0098B420).
// Shape: the pool array-new guard is the nested conditional of the landed 0x0098B420 (count ? (64-bit product >
//   0xFFFFFFFF ? 0 : Allocate(count * 16)) : 0).
struct C4DVector { float X, Y, Z, W; };

struct CSimpleMemoryPool {
    unsigned char _pad[0x1c];
    void FreeAll();                                // 0x00A5C720
    void* Allocate(unsigned long bytes);           // 0x00A5C910
};

template <class T> inline T* PoolNewArray(CSimpleMemoryPool& pool, unsigned long count)
{
    return count != 0 ? ((unsigned __int64)count * sizeof(T) > 0xffffffff ? 0 : (T*)pool.Allocate(count * sizeof(T))) : 0;
}

class CEngineClothRenderer {
public:
    unsigned char     _pad000[0xa8];
    CSimpleMemoryPool PositionBufferPool;          // +0xa8
    C4DVector*        PositionBuffer;              // +0xc4
    unsigned long     PositionBufferSize;          // +0xc8
    CSimpleMemoryPool NormalBufferPool;            // +0xcc
    C4DVector*        NormalBuffer;                // +0xe8
    unsigned long     NormalBufferSize;            // +0xec
    C4DVector* GetPositionBuffer(unsigned long size);
};

C4DVector* CEngineClothRenderer::GetPositionBuffer(unsigned long size)
{
    if (size > PositionBufferSize) {
        PositionBufferPool.FreeAll();
        PositionBuffer = PoolNewArray<C4DVector>(PositionBufferPool, size);
        PositionBufferSize = size;
    }
    return PositionBuffer;
}