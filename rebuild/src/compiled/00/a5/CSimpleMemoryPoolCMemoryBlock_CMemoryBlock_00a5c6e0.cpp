// CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock() -- this class's vtable (0x0129D568), the block at +8 released (0x009F2B20, `__fastcall`) when owned (+0xc), then the CIVCountedPointeeBase base's inline destructor stores its vtable (0x012354D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C6E0, length 32 bytes.
// Leaf: ??1CMemoryBlock@CSimpleMemoryPool@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class, the owned byte tested before the vtable store, `ret`.
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
    ~CMemoryBlock();
};
}

CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock()
{
    if (m_owned)
        DeallocateAligned_009f2b20(m_pData);
}