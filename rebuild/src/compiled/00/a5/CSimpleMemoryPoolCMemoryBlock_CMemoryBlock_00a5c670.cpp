// CSimpleMemoryPool::CMemoryBlock::CMemoryBlock(unsigned long size) -- the CIVCountedPointeeBase base inline (reference count 1), this class's vtable (0x0129D568), the size stored (+0x10), the used count cleared (+0x14), the block allocated 16-aligned (0x009F2BE0, `__fastcall(size, 16)`) into +8 and marked owned (+0xc)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C670, length 53 bytes.
// Callees: AllocateAligned @ 0x009F2BE0. Shape: `this` in esi, the size moved to ecx and the alignment to edx before the stores, `ret 4`.
void* __fastcall AllocateAligned_009f2be0(unsigned long size, unsigned long alignment);   // 0x009F2BE0
void __fastcall DeallocateAligned_009f2b20(void* block);   // 0x009F2B20

struct CIVCountedPointeeBase {                     // vtable 0x012354D0
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                      // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
    ~CIVCountedPointeeBase() { }                   // inline: the vtable store
};

namespace CSimpleMemoryPool {
struct CMemoryBlock : CIVCountedPointeeBase {      // vtable 0x0129D568
    void*         m_pData;                         // +0x08
    bool          m_owned;                         // +0x0c
    unsigned long m_size;                          // +0x10
    long          m_used;                          // +0x14
    CMemoryBlock(unsigned long size);
    
};
}

CSimpleMemoryPool::CMemoryBlock::CMemoryBlock(unsigned long size)
    : CIVCountedPointeeBase(), m_size(size), m_used(0)
{
    m_pData = AllocateAligned_009f2be0(size, 16);
    m_owned = true;
}