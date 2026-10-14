// CHandleBasedMemoryPool::InternalAllocateHandle() -- pop the free-handle chain (+0x44c threads free slots of the handle vector at +0x43c through the slots themselves; -1 ends it): the popped slot is marked -1 and its index returned; otherwise grow the vector by one null slot (push_back @ 0x00C29C30) and return the old size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28A80, length 96 bytes.
// Shape: a 4-byte frame for the pushed null, the chain pop through `lea edx,[edx+eax*4]`, `add ecx,43Ch` for the vector `this`, the null-checked size in esi.
struct SFreeArea_00c28a80;
struct CHandleVector_00c28a80 {                     // +0x43c: a 16-byte vector of area pointers
    unsigned char       _pad[4];
    SFreeArea_00c28a80** m_pFirst;                 // +0x04 (pool +0x440)
    SFreeArea_00c28a80** m_pLast;                  // +0x08 (pool +0x444)
    SFreeArea_00c28a80** m_pEnd;                   // +0x0c
    void PushBack(SFreeArea_00c28a80* const& value);   // 0x00C29C30 (labelled "CParticleProgram::AddTextureCoord")
    __forceinline long Size() { if (m_pFirst == 0) return 0; return m_pLast - m_pFirst; }
    __forceinline SFreeArea_00c28a80*& At(long index) { return m_pFirst[index]; }
};
struct CHandleBasedMemoryPool {
    unsigned char          _pad[0x43c];
    CHandleVector_00c28a80 m_handles;              // +0x43c
    long                   m_firstFreeHandle;      // +0x44c (-1 = none)
    long InternalAllocateHandle();
};

long CHandleBasedMemoryPool::InternalAllocateHandle()
{
    long index = m_firstFreeHandle;
    if (index != -1) {
        SFreeArea_00c28a80*& slot = m_handles.At(index);
        m_firstFreeHandle = (long)slot;             // the next free slot threads through the slot
        slot = (SFreeArea_00c28a80*)-1;
        return index;
    }
    long count = m_handles.Size();
    SFreeArea_00c28a80* empty = 0;
    m_handles.PushBack(empty);
    return count;
}