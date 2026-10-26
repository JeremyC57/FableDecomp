// std::__destroy_aux(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, const __false_type&) -- run the inlined ~CBufferChunk over [first, last): give each chunk's memory back to the global allocator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6D60, length 123 bytes (padded to 128).
// Leaf: __destroy_aux
// Label: the catalog "_Destroy_range<std::allocator<CFileInstaller::CBufferChunk>>" names the element correctly: the
// stride is 8 = Ego_r.pdb CBufferChunk {Mem +0, Used +4}; as an STLport out-of-line body it is `__destroy_aux(first,
// last, const __false_type&)` (nine callers in the CFileInstaller code, 0x009C52E7 ...).
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate
//          @ 0x009F2920 / GetAllocationCount @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / operator delete @ 0x009F1F90.
// Globals: the global allocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94.
// Shape: the chunk destructor tests Mem and calls the landed GFFreeFromGlobalAllocator body (0x009A4350) inline, whose
// own null test survives (`test esi,esi` twice) because it sits after the lock call.
class CMemoryAllocator { public:
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;          // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94; // 0x013CAA94
__forceinline void GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}
class CFileInstaller { public:
    class CBufferChunk { public:                          // Ego_r.pdb, 8 bytes
        unsigned char* Mem;                        // +0x00
        bool           Used;                       // +0x04
        ~CBufferChunk()
        {
            if (Mem)
                GFFreeFromGlobalAllocator(Mem);
        }
    };
};
namespace std {
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}