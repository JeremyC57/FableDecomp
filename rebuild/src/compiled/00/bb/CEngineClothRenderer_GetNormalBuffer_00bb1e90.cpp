// CEngineClothRenderer::GetNormalBuffer(unsigned long size) -- return the cloth vertex-normal buffer, regrowing it from its frame pool (old contents dropped) when more than the current size is asked for
// Leaf: GetNormalBuffer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1E90, 93 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb ?GetNormalBuffer@CEngineClothRenderer@@AAEPAVC4DVector@@K@Z (private); the one caller is
//   CEngineClothRenderer::CalculateVertexNormals 0x00BB1F76. Twin of GetPositionBuffer 0x00BB1E30. Catalog
//   "VertexBuffer_Reallocate" is descriptive.
// Callees: CSimpleMemoryPool::FreeAll 0x00A5C720, CSimpleMemoryPool::Allocate(unsigned long) 0x00A5C910 (both landed).
// Layout: Ego_r.pdb CEngineClothRenderer less 0x18 (six 12-byte retail CArrays, PDB 16): PositionBufferPool +0xA8,
//   PositionBuffer +0xC4, PositionBufferSize +0xC8, NormalBufferPool +0xCC, NormalBuffer +0xE8, NormalBufferSize +0xEC. There is no engine header for the class; the pool needs its member
//   functions, so both are local models (as the landed pool callers 0x0098B420).
// Shape: the pool array-new guard is the nested conditional of the landed 0x0098B420 (count ? (64-bit product >
//   0xFFFFFFFF ? 0 : Allocate(count * 16)) : 0).
class C4DVector { public: float X, Y, Z, W; };

class CSimpleMemoryPool { public:
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
private:
    C4DVector* GetNormalBuffer(unsigned long size);
};

C4DVector* CEngineClothRenderer::GetNormalBuffer(unsigned long size)
{
    if (size > NormalBufferSize) {
        NormalBufferPool.FreeAll();
        NormalBuffer = PoolNewArray<C4DVector>(NormalBufferPool, size);
        NormalBufferSize = size;
    }
    return NormalBuffer;
}