// CHandleBasedMemoryPool::Allocate(CPoolHandle& handle, long alignment, long size) -- allocate `size` bytes through the Allocate(long*, unsigned long, unsigned long) overload (0x00A03180, the index returned in the `size` slot); on success release whatever the handle held (Free @ 0x00A02B30 on its pool, the pool cleared and the index set to -1) and bind it to this pool and the new index; false when the allocation failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A032F0, length 84 bytes.
// Shape: `this` in edi, the handle in esi, `ret 0Ch`, `mov al,1` / `xor al,al`.
class CHandleBasedMemoryPool;
struct CPoolHandle_00a032f0 {                      // 8 bytes: the owning pool and the index into it
    CHandleBasedMemoryPool* m_pPool;               // +0x00
    long                    m_index;               // +0x04
};
class CHandleBasedMemoryPool { public:
    bool Allocate(long* index, unsigned long alignment, unsigned long size);   // 0x00A03180 (NF306: Ego_r + FableWin ?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z; was InternalAllocate(long&, long, long))
    void Free(long index);                                           // 0x00A02B30
    bool Allocate(CPoolHandle_00a032f0& handle, long alignment, long size);
};

bool CHandleBasedMemoryPool::Allocate(CPoolHandle_00a032f0& handle, long alignment, long size)
{
    if (Allocate(&size, alignment, size)) { // the index comes back in the (dead) size slot
        if (handle.m_pPool) {
            handle.m_pPool->Free(handle.m_index);
            handle.m_pPool = 0;
            handle.m_index = -1;
        }
        handle.m_pPool = this;
        handle.m_index = size;
        return true;
    }
    return false;
}