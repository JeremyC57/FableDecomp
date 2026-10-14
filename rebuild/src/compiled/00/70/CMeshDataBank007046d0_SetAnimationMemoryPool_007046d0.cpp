// CMeshDataBank_007046d0::SetAnimationMemoryPool(CCountedPointer pool) -- assign the counted pointer at +4/+8 from the one passed BY VALUE (when the count blocks differ: the old block released, the object and block stored, the new block's references incremented), then the by-value argument's own destructor runs (its block released)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007046D0, length 96 bytes.
// Leaf: SetAnimationMemoryPool
// Callees: operator delete @ 0x00BFE9BC. The chunk label is propagated; named by address. Shape: the argument's two words read from the stack, its block in edi, `ret 8` (the callee cleans the by-value argument).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CPool_007046d0;
struct SCountBlock_007046d0 {                      // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CPool_007046d0*);   // +0x04
    CPool_007046d0* m_pObject;                     // +0x08
};
struct CCountedPointer_007046d0 {                  // {object, count block}
    CPool_007046d0*       m_pObject;               // +0x00
    SCountBlock_007046d0* m_pCount;                // +0x04
    CCountedPointer_007046d0(const CCountedPointer_007046d0& other);
    __forceinline ~CCountedPointer_007046d0()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    __forceinline CCountedPointer_007046d0& operator=(const CCountedPointer_007046d0& other)
    {
        SCountBlock_007046d0* block = other.m_pCount;
        if (m_pCount != block) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = other.m_pObject;
            m_pCount = block;
            if (block)
                ++block->m_references;
        }
        return *this;
    }
};

struct CMeshDataBank_007046d0 {
    long                     m_0;
    CCountedPointer_007046d0 m_pool;               // +0x04
    void SetAnimationMemoryPool(CCountedPointer_007046d0 pool);
};

void CMeshDataBank_007046d0::SetAnimationMemoryPool(CCountedPointer_007046d0 pool)
{
    m_pool = pool;
}