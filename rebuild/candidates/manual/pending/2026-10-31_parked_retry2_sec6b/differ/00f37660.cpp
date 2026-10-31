// CGPUScratchBufferTextureManager::AllocteMemoryPage(long scratchPool, bool enableAutoRestore) -- open a memory page over
// Grade: DIFFER(ours v retail) -- 234v206: retail keeps the pool's buffer/size in ebp/ebx across the vector<long>::operator= call and drops the local page's list dtor on the reuse path (it knows the call leaves the local untouched); our build reloads the fields from the frame and keeps the free() dtor; a same-TU auto_inline(off) operator= body (malloc/memmove form) did not change it
// a whole scratch pool, reusing a free page slot when there is one, else appending; returns the page index
// Leaf: AllocteMemoryPage
// Retry sec6b: catalog extent 776 over-spans (ret 8 at +0xcb, int3 pad); graded over 206 (oracle_extra). The pool's size and buffer
//   read into locals and the reused slot written field by field from them. Residue: retail drops the local page's list dtor on
//   the reuse path and forwards the page fields across the vector<long>::operator= call (inter-procedural mod/ref, LTCG lane gap);
//   esi/edi/ebx/ebp assignment.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37660, length 206 bytes, `ret 8`.
// Callees: vector<long>::operator= @ 0x00454886 (an ICF-shared body), vector<CResourcePage>::push_back @ 0x00F38860,
// free @ 0x00BFEA14 (the local page's list storage). Graphics layer.
// Retail layout: see 0x00F372B0 (MemoryPages +0x10 of 0x1C-byte CResourcePage, ScratchPools +0x1C of {Buffer,
// BufferSize}, FreeTexturePageSlots +0x28).
// Shape: the page is a local built member by member (the pool's buffer and size, an empty STLport list, the bool);
// a reused slot is filled by CResourcePage's inline memberwise operator= (the list through vector<long>::operator=),
// after which VC knows the local list is still empty and drops its destructor; the append path destroys it (`free`
// of a non-null start).

extern "C" void __cdecl free(void* p);             // 0x00BFEA14

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; }
    CRetailVector& operator=(const CRetailVector& other);   // 0x00454886 (vector<long>)
    void push_back(const T& x);                              // 0x00F38860 (vector<CResourcePage>)
};

class CGPUScratchBufferTextureManager {
public:
    struct CScratchPool {
        unsigned char* Buffer;
        long BufferSize;
    };
    struct CResourcePage {
        long ScratchPool;                           // +0x00
        CRetailVector<long> ResourceList;           // +0x04
        unsigned char* NextFreeBufferAddress;       // +0x10
        long FreeSpaceSize;                         // +0x14
        bool EnableAutoRestore;                     // +0x18
        CResourcePage& operator=(const CResourcePage& other)
        {
            ScratchPool = other.ScratchPool;
            ResourceList = other.ResourceList;
            NextFreeBufferAddress = other.NextFreeBufferAddress;
            FreeSpaceSize = other.FreeSpaceSize;
            EnableAutoRestore = other.EnableAutoRestore;
            return *this;
        }
    };

    virtual ~CGPUScratchBufferTextureManager();
    unsigned char _pad04[0xc];
    CRetailVector<CResourcePage> MemoryPages;       // +0x10
    CRetailVector<CScratchPool> ScratchPools;       // +0x1C
    CRetailVector<long> FreeTexturePageSlots;       // +0x28
    long AllocteMemoryPage(long scratchPool, bool enableAutoRestore);
};

long CGPUScratchBufferTextureManager::AllocteMemoryPage(long scratchPool, bool enableAutoRestore)
{
    CScratchPool& pool = ScratchPools[scratchPool];
    long bufferSize = pool.BufferSize;
    unsigned char* buffer = pool.Buffer;
    CResourcePage page;
    page.ScratchPool = scratchPool;
    page.NextFreeBufferAddress = buffer;
    page.FreeSpaceSize = bufferSize;
    page.EnableAutoRestore = enableAutoRestore;
    if (FreeTexturePageSlots.size() > 0) {
        long index = FreeTexturePageSlots[FreeTexturePageSlots.size() - 1];
        FreeTexturePageSlots.pop_back();
        CResourcePage& slot = MemoryPages[index];
        slot.ScratchPool = scratchPool;
        slot.ResourceList = page.ResourceList;
        slot.NextFreeBufferAddress = buffer;
        slot.FreeSpaceSize = bufferSize;
        slot.EnableAutoRestore = enableAutoRestore;
        return index;
    }
    long index = MemoryPages.size();
    MemoryPages.push_back(page);
    return index;
}
