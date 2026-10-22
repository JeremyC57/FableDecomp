// C3DAnimationManager::CalcBlendedMeshTransformations(const C3DAnimationBlendState*, const C3DAnimationBlendState*, float, C3DVector*, const C3DMesh2*, CMatrix3x4*, unsigned long) -- blend into a pooled 4x4 array and convert each bone matrix to 3x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA09F0, length 608 bytes (`ret 1Ch`).
// Callees: CSimpleMemoryPool::Allocate @ 0x00A5C910, the CMatrix4x4 overload of CalcBlendedMeshTransformations
//          @ 0x00AA0090 (with the count and `false`), CSimpleMemoryPool::FreeAll @ 0x00A5C720.
// Shape: the bone count (mesh +0x98) sizes a pool array of 64-byte CMatrix4x4 (the pool guard is the landed nested
// conditional count ? (64-bit product > 0xFFFFFFFF ? 0 : Allocate) : 0, 0x0098B420); after the 4x4 blend each matrix is
// narrowed to 3x4 (the first three floats of each row) in a loop VC7.1 unrolls by four; the pool is reset at the end.
// The last argument is unused. The narrowing is written with a named destination reference bound BEFORE the source
// reference (a Set(const CMatrix4x4&) member swaps the two induction registers).
// Layout: C3DAnimationManager frame pool +0x30; C3DMesh2 bone count +0x98.
// Leaf: ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXPBVC3DAnimationBlendState@@0MPAVC3DVector@@PBVC3DMesh2@@PAVCMatrix3x4@@K@Z
struct C3DVector { float X, Y, Z; };
struct C3DVector4 { float X, Y, Z, W; };
struct CMatrix4x4 { C3DVector4 Row[4]; };
struct CMatrix3x4 {
    C3DVector Row[4];
};
struct CSimpleMemoryPool {
    void* Allocate(unsigned long bytes);           // 0x00A5C910
    void FreeAll();                                // 0x00A5C720
};
template <class T> inline T* PoolNewArray(CSimpleMemoryPool& pool, unsigned long count)
{
    return count != 0 ? ((unsigned __int64)count * sizeof(T) > 0xffffffff ? 0 : (T*)pool.Allocate(count * sizeof(T))) : 0;
}
struct C3DAnimationBlendState;
struct C3DMesh2 {
    unsigned char _pad[0x98];
    unsigned long NumBones;                        // +0x98
};
struct C3DAnimationManager {
    unsigned char _pad[0x30];
    CSimpleMemoryPool MemoryPool;                  // +0x30
    void CalcBlendedMeshTransformations(const C3DAnimationBlendState* a, const C3DAnimationBlendState* b, float t,
        C3DVector* movement, const C3DMesh2* mesh, CMatrix4x4* out, unsigned long count, bool flag);   // 0x00AA0090
    void CalcBlendedMeshTransformations(const C3DAnimationBlendState* a, const C3DAnimationBlendState* b, float t,
        C3DVector* movement, const C3DMesh2* mesh, CMatrix3x4* out, unsigned long unused);
};

void C3DAnimationManager::CalcBlendedMeshTransformations(const C3DAnimationBlendState* a, const C3DAnimationBlendState* b, float t,
    C3DVector* movement, const C3DMesh2* mesh, CMatrix3x4* out, unsigned long unused)
{
    unsigned long count = mesh->NumBones;
    CMatrix4x4* blended = PoolNewArray<CMatrix4x4>(MemoryPool, count);
    CalcBlendedMeshTransformations(a, b, t, movement, mesh, blended, count, false);
    for (unsigned long i = 0; i < count; ++i)
    {
        CMatrix3x4& dest = out[i];
        const CMatrix4x4& src = blended[i];
        dest.Row[0].X = src.Row[0].X; dest.Row[0].Y = src.Row[0].Y; dest.Row[0].Z = src.Row[0].Z;
        dest.Row[1].X = src.Row[1].X; dest.Row[1].Y = src.Row[1].Y; dest.Row[1].Z = src.Row[1].Z;
        dest.Row[2].X = src.Row[2].X; dest.Row[2].Y = src.Row[2].Y; dest.Row[2].Z = src.Row[2].Z;
        dest.Row[3].X = src.Row[3].X; dest.Row[3].Y = src.Row[3].Y; dest.Row[3].Z = src.Row[3].Z;
    }
    MemoryPool.FreeAll();
}
