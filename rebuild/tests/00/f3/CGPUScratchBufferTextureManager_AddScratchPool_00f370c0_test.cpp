// Behaviour test for 00f370c0: src/00f370c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGPUScratchBufferTextureManager::AddScratchPool(long size)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGPUScratchBufferTextureManager::AddScratchPool(long size) -- append a scratch memory pool (a global-allocator
// buffer of `size` bytes when rendering headless, else an empty pool) and return its index
// Leaf: AddScratchPool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F370C0, length 166 bytes (padded to 176), `ret 4`.
// Callees: GFCreateGlobalAllocator @ 0x009F2870, GFEnterAllocatorLock @ 0x009F2000, CMemoryAllocator::InternalAllocate
// @ 0x009F2A40 (alignment/flags 9), GFLeaveAllocatorLock @ 0x009F2010, the STLport vector<CScratchPool>
// _M_insert_overflow @ 0x00F380E0. Globals: g_HeadlessRendering byte 0x0143E968 (landed 0x00F37440's name), the global
// allocator 0x013CAA90. Graphics layer.
// Retail layout (PDB names; the PDB CArrays are 16 bytes, retail STLport vectors 12): Resources +4, MemoryPages +0x10,
// ScratchPools +0x1C (CScratchPool {Buffer, BufferSize}, 8 bytes), FreeTexturePageSlots +0x28, FreeTextureSlots
// +0x34, ShutdownFlag +0x40.
// Shape: the pool local filled per arm; push_back is the inline STLport form (placement-new null test, the
// overflow helper with the __true_type tag in the dead `size` slot); the index is `size() - 1`.

class CMemoryAllocator { public:
    void* InternalAllocate(long size, unsigned long flags, bool flag);   // 0x009F2A40
};
extern CMemoryAllocator* g_pGlobalAllocator;        // 0x013CAA90
extern char g_HeadlessRendering;                    // 0x0143E968
void __cdecl GFCreateGlobalAllocator();             // 0x009F2870
void __cdecl GFEnterAllocatorLock();                // 0x009F2000
void __cdecl GFLeaveAllocatorLock();                // 0x009F2010

struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00F380E0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)

class CGPUScratchBufferTextureManager {
public:
    struct CScratchPool {
        unsigned char* Buffer;
        long BufferSize;
    };
    virtual ~CGPUScratchBufferTextureManager();
    unsigned char _pad04[0x18];
    CRetailVector<CScratchPool> ScratchPools;       // +0x1C
    long AddScratchPool(long size);
};

long CGPUScratchBufferTextureManager::AddScratchPool(long size)
{
    CScratchPool pool;
    if (g_HeadlessRendering) {
        if (!g_pGlobalAllocator)
            GFCreateGlobalAllocator();
        GFEnterAllocatorLock();
        unsigned char* buffer = (unsigned char*)g_pGlobalAllocator->InternalAllocate(size, 9, false);
        GFLeaveAllocatorLock();
        pool.Buffer = buffer;
        pool.BufferSize = size;
    } else {
        pool.Buffer = 0;
        pool.BufferSize = 0;
    }
    ScratchPools.push_back(pool);
    return ScratchPools.size() - 1;
}

// ---- stubs and driver ----
static CGPUScratchBufferTextureManager::CScratchPool g_pools[4]; static int g_grow = 0, g_create = 0, g_enter = 0, g_leave = 0, g_alloc = 0;
static long g_allocSize = 0; static unsigned long g_allocFlags = 0; static bool g_allocFlag = true;
static unsigned char g_buffer[16];
CMemoryAllocator g_allocator;
CMemoryAllocator* g_pGlobalAllocator = 0;
char g_HeadlessRendering = 0;
void __cdecl GFCreateGlobalAllocator() { ++g_create; g_pGlobalAllocator = &g_allocator; }
void __cdecl GFEnterAllocatorLock() { ++g_enter; CHECK(g_enter == g_leave + 1); }
void __cdecl GFLeaveAllocatorLock() { ++g_leave; CHECK(g_enter == g_leave); }
void* CMemoryAllocator::InternalAllocate(long size, unsigned long flags, bool flag) { ++g_alloc; CHECK(this == &g_allocator); CHECK(g_enter == g_leave + 1); g_allocSize = size; g_allocFlags = flags; g_allocFlag = flag; return g_buffer; }
CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager() {}
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CRetailVector<CGPUScratchBufferTextureManager::CScratchPool>* v = (CRetailVector<CGPUScratchBufferTextureManager::CScratchPool>*)vector;
    CGPUScratchBufferTextureManager::CScratchPool* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
int main()
{
    CGPUScratchBufferTextureManager m; m.ScratchPools._M_start = g_pools; m.ScratchPools._M_finish = g_pools; m.ScratchPools._M_end_of_storage = g_pools + 1;
    CHECK(m.AddScratchPool(64) == 0); CHECK(g_pools[0].Buffer == 0 && g_pools[0].BufferSize == 0); CHECK(g_alloc == 0 && g_create == 0 && g_enter == 0); CHECK(g_grow == 0);
    g_HeadlessRendering = 1;
    CHECK(m.AddScratchPool(128) == 1); CHECK(g_create == 1 && g_enter == 1 && g_leave == 1 && g_alloc == 1); CHECK(g_allocSize == 128 && g_allocFlags == 9 && g_allocFlag == false);
    CHECK(g_pools[1].Buffer == g_buffer && g_pools[1].BufferSize == 128); CHECK(g_grow == 1);
    m.ScratchPools._M_end_of_storage = g_pools + 4;
    CHECK(m.AddScratchPool(32) == 2); CHECK(g_create == 1 && g_alloc == 2 && g_leave == 2); CHECK(g_pools[2].Buffer == g_buffer && g_pools[2].BufferSize == 32); CHECK(g_grow == 1);
    if (fails == 0) printf("00f370c0_TEST PASS\n"); else printf("00f370c0_TEST FAIL\n");
    return fails;
}