// CGPUScratchBufferTextureManager::AddScratchPool(long size) -- append a scratch memory pool (a global-allocator
// Grade: DIFFER(ours v retail) -- 171v167: retail carries the pool size in ecx out of both arms and stores the pool words once after the join (buffer in edi, size param in esi); ours stores BufferSize inside each arm and reloads it for the in-place copy; the overflow call's tag/value lea registers are swapped (per-arm, split-local, ctor and declaration-order forms tried)
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

struct CMemoryAllocator {
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
