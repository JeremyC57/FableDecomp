// CMemoryAllocatorFixedSize::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129DB64), then for flag bit 0 the object released through the class's `__fastcall` operator delete (0x00A66750: VirtualFree(this, 0, MEM_RELEASE))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66600, length 27 bytes.
// Leaf: ??_GCMemoryAllocatorFixedSize@@UAEPAXI@Z
// Shape: the flag tested before the vtable store, `this` in esi passed in ecx to the release, `ret 4`.
void __fastcall ReleaseAllocator_00a66750(void* block);   // 0x00A66750: VirtualFree(block, 0, MEM_RELEASE)

struct CMemoryAllocatorFixedSize {                 // vtable 0x0129DB64
    virtual void v00();
    unsigned char _pad04[0xc30 - 4];
    __forceinline virtual ~CMemoryAllocatorFixedSize() { }
    static void operator delete(void* block) { ReleaseAllocator_00a66750(block); }
};

void* MakeOne_00a66600()                           // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CMemoryAllocatorFixedSize;
}
