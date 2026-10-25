// CMemoryAllocator::DestroyDeallocator() -- the dynamic atexit destructor for the static `CMemoryAllocator::Deallocator`: at exit flag the allocator as shutting down and, when no sub-allocators are left, destroy the global allocator and release its pages
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A9A0, 61 bytes (+3 int3), in `.text$yd`; its dynamic initializer
//   @ 0x01224240 only registers it with atexit (CDeallocator is empty). VC7.1 names the pair `$E1` / `$E2` in the TU.
// Identity: FableWin.pdb lists "`dynamic atexit destructor for 'CMemoryAllocator::Deallocator''" after the console's
//   OPEN_BRACKET / CLOSE_BRACKET / SEPARATOR strings, as retail's thunk follows three CCharString thunks
//   (0x0122A8D0..0x0122A8F0); `??1CDeallocator@CMemoryAllocator@@QAE@XZ`, `?Destroy@CMemoryAllocator@@KIXXZ`,
//   `??3CMemoryAllocator@@KIXPAX@Z` and `?PeekNoAllocators@CMemoryAllocator@@QBEJXZ` are its PDB members. The same
//   teardown appears out of line in UninitializeStreamingFontTextureCache 0x009A16C9 (shutting-down flag, a zero
//   PeekNoAllocators, then ~CMemoryAllocator + operator delete 0x009F1F90 and the pointer cleared). The globals are the
//   allocator instance 0x013CAA90 and the shutting-down flag 0x013CAA94; their names and the +0x40C member are
//   descriptive (named after the PDB accessors). The catalog label "Destroy" names the inlined callee.
// Callees: ~CMemoryAllocator @ 0x009F2310, VirtualFree (KERNEL32 import [0x0143FD40]) through the class's operator delete.
// Shape: ~CDeallocator -> Destroy() -> `delete PAllocator` all inline: the null test of the delete folds into the outer
//   one (the pointer stays in ecx, then esi), the flag store does not force a reload, and the class operator delete is
//   `VirtualFree(p, 0, MEM_RELEASE)`.
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);   // [0x0143FD40]

class CMemoryAllocator {
public:
    ~CMemoryAllocator();                                  // 0x009F2310
    long PeekNoAllocators() const { return NoAllocators; }

    class CDeallocator {
    public:
        ~CDeallocator();
    };
    static CDeallocator Deallocator;

protected:
    static void __fastcall Destroy();
    static void __fastcall operator delete(void* p) { VirtualFree(p, 0, 0x8000 /* MEM_RELEASE */); }

    unsigned char _pad000[0x40c];
    long NoAllocators;                                    // +0x40C

    static CMemoryAllocator* PAllocator;                  // 0x013CAA90
    static bool ShuttingDown;                             // 0x013CAA94
};

void __fastcall CMemoryAllocator::Destroy()
{
    delete PAllocator;
    PAllocator = 0;
}

CMemoryAllocator::CDeallocator::~CDeallocator()
{
    if (PAllocator) {
        ShuttingDown = true;
        if (PAllocator->PeekNoAllocators() == 0)
            Destroy();
    }
}

CMemoryAllocator::CDeallocator CMemoryAllocator::Deallocator;