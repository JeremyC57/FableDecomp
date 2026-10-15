// Behaviour test for 0x00A901E0 (C3DPrimitive2::FreeData): every owned resource is released and the bookkeeping cleared.
#include <cstdio>
#include <cstring>
static int g_deleted = 0, g_blocksDestroyed = 0;
struct Resource { virtual ~Resource() { ++g_deleted; } };
template <class T> struct ScopedPtr { T* m_p; void reset() { T* old = m_p; m_p = 0; if (old) delete old; } };
template <class T> struct Array {
    T* m_first; T* m_last; T* m_end;
    static T* CopyTrivial(const T* first, const T* last, T* result) { if (first == last) return result; size_t n = (const char*)last - (const char*)first; return (T*)((char*)std::memmove(result, first, n) + n); }
    void clear() { m_last = CopyTrivial(m_last, m_last, m_first); }
};
static void DestroyAnimatedBlocks(int* blocks, unsigned int count) { if (blocks) g_blocksDestroyed += (int)count; }
struct Primitive {
    ScopedPtr<Resource> Data, VertexBuffer, IndexBuffer; unsigned int VertexBufferStatus, IndexBufferStatus, StaticBlockCount; int* StaticBlocks;
    unsigned int AnimatedBlockCount; int* AnimatedBlocks; unsigned int AnimatedBlocksStatus; Array<unsigned short> Partitioned;
    void FreeData()
    {
        Partitioned.clear(); Data.reset(); VertexBuffer.reset(); VertexBufferStatus = 0; IndexBuffer.reset(); IndexBufferStatus = 0;
        DestroyAnimatedBlocks(AnimatedBlocks, AnimatedBlockCount); StaticBlocks = 0; AnimatedBlocks = 0; AnimatedBlocksStatus = 0;
    }
};
int main()
{
    unsigned short idx[4] = { 1, 2, 3, 4 }; int blocks[3];
    Primitive p; p.Data.m_p = new Resource; p.VertexBuffer.m_p = new Resource; p.IndexBuffer.m_p = 0;
    p.VertexBufferStatus = 3; p.IndexBufferStatus = 4; p.StaticBlocks = blocks; p.AnimatedBlockCount = 3; p.AnimatedBlocks = blocks; p.AnimatedBlocksStatus = 1;
    p.Partitioned.m_first = idx; p.Partitioned.m_last = idx + 4; p.Partitioned.m_end = idx + 4;
    p.FreeData();
    int ok = g_deleted == 2 && !p.Data.m_p && !p.VertexBuffer.m_p && !p.IndexBuffer.m_p && p.VertexBufferStatus == 0 && p.IndexBufferStatus == 0;
    ok = ok && g_blocksDestroyed == 3 && !p.StaticBlocks && !p.AnimatedBlocks && p.AnimatedBlocksStatus == 0 && p.Partitioned.m_last == p.Partitioned.m_first;
    if (!ok) { std::printf("00a901e0_TEST FAIL\n"); return 1; }
    std::printf("00a901e0_TEST PASS\n");
    return 0;
}
