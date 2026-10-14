// CSimpleMemoryPool::CMemoryBlock::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129D568), the block at +8 released (0x009F2B20, `__fastcall`) when owned (+0xc), the CIVCountedPointeeBase base's inline destructor stores its vtable (0x012354D0), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C9B0, length 52 bytes.
// Leaf: ??_GCMemoryBlock@CSimpleMemoryPool@@UAEPAXI@Z
// Shape: the owned byte tested before the vtable store, the flag tested before the base vtable store, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
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
    CMemoryBlock(unsigned long size) : CIVCountedPointeeBase(), m_size(size), m_used(0) { m_pData = AllocateAligned_009f2be0(size, 16); m_owned = true; }   // inline here so `new` pulls the vtable (and the ??_G) into this object
    __forceinline virtual ~CMemoryBlock() { if (m_owned) DeallocateAligned_009f2b20(m_pData); }
};
}

void* MakeOne_00a5c9b0()                           // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CSimpleMemoryPool::CMemoryBlock(16);
}