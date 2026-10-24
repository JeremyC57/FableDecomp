// Behaviour test for 009c6d60 (std::__destroy_aux over CFileInstaller::CBufferChunk): each chunk with memory returns it to the global allocator under the allocator lock; the allocator is destroyed once it is empty (when so flagged).
#include <cstdio>
#include <new>
static int g_deallocated = 0;
static int g_locks = 0;
static int g_unlocks = 0;
static int g_allocatorsDestroyed = 0;
struct CMemoryAllocator {
    long m_count;
    void InternalDeallocate(void*) { ++g_deallocated; --m_count; }   // stands in for 0x009F2920
    long GetAllocationCount() { return m_count; }                     // stands in for 0x009F1F10
    ~CMemoryAllocator() { ++g_allocatorsDestroyed; }                  // stands in for 0x009F2310
};
static void GFEnterAllocatorLock() { ++g_locks; }
static void GFLeaveAllocatorLock() { ++g_unlocks; }
static CMemoryAllocator* g_pGlobalAllocator = 0;
static bool g_bDestroyAllocatorWhenEmpty = true;
inline void GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator) {
        g_pGlobalAllocator->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty) {
            if (g_pGlobalAllocator->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator;
                g_pGlobalAllocator = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}
struct CFileInstaller {
    struct CBufferChunk {
        unsigned char* Mem;
        bool           Used;
        ~CBufferChunk()
        {
            if (Mem)
                GFFreeFromGlobalAllocator(Mem);
        }
    };
};
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __destroy_aux(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
int main()
{
    static unsigned char a[4], b[4];
    g_pGlobalAllocator = new CMemoryAllocator;
    g_pGlobalAllocator->m_count = 2;
    union { void* p; char bytes[sizeof(CFileInstaller::CBufferChunk) * 3]; } storage;
    CFileInstaller::CBufferChunk* chunks = reinterpret_cast<CFileInstaller::CBufferChunk*>(storage.bytes);
    chunks[0].Mem = a; chunks[0].Used = true;
    chunks[1].Mem = 0; chunks[1].Used = false;
    chunks[2].Mem = b; chunks[2].Used = true;
    __false_type tag;
    __destroy_aux(chunks, chunks, tag);
    if (g_locks != 0) { std::printf("009c6d60_TEST FAIL\n"); return 1; }
    __destroy_aux(chunks, chunks + 3, tag);
    if (g_deallocated != 2 || g_locks != 2 || g_unlocks != 2) { std::printf("009c6d60_TEST FAIL\n"); return 1; }
    if (g_allocatorsDestroyed != 1 || g_pGlobalAllocator != 0) { std::printf("009c6d60_TEST FAIL\n"); return 1; }
    std::printf("009c6d60_TEST PASS\n");
    return 0;
}