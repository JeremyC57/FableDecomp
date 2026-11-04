// CGPUScratchBufferTextureManager::AllocteMemoryPage(long scratchPool, bool enableAutoRestore) -- open a memory page over a whole scratch pool, reusing a free page slot when there is one, else appending; returns the page index
// Leaf: ?AllocteMemoryPage@CGPUScratchBufferTextureManager@@QAEJJ_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37660, length 206 bytes, `ret 8`. Identity: Ego_r.pdb / FableWin.pdb
//   ?AllocteMemoryPage@CGPUScratchBufferTextureManager@@QAEJJ_N@Z (the PDB spelling, "Allocte").
// Callees: vector<long>::operator= 0x00454886 (an ICF-shared body), vector<CResourcePage>::push_back 0x00F38860, free 0x00BFEA14
//   (the local page's list storage, the STLport malloc allocator). Graphics layer.
// Layout: Ego_r.pdb CGPUScratchBufferTextureManager {vptr, Resources, MemoryPages, ScratchPools, FreeTexturePageSlots,
//   FreeTextureSlots: CArray<...>, ShutdownFlag} at retail 12-byte CArrays (MemoryPages +0x10, ScratchPools +0x1C,
//   FreeTexturePageSlots +0x28); CResourcePage {ScratchPool, ResourceList CArray<long>, NextFreeBufferAddress, FreeSpaceSize,
//   EnableAutoRestore} (0x1C bytes); CScratchPool {Buffer, BufferSize}. No engine header for the class: local model.
// Shape: the page is a local built member by member; a reused slot gets the implicit memberwise operator= (the list through
//   vector<long>::operator=, defined in this TU by FABLE_RETAIL_STLP_VECTOR_C as retail instantiated it here, so VC7.1 knows the
//   call leaves the local untouched: the pool's buffer/size stay in ebp/ebx and the local list's destructor folds away on that
//   path); the append path destroys the local (`free` of a non-null start).
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"

class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry;
    struct CScratchPool {
        unsigned char* Buffer;                          // +0x00
        long BufferSize;                                // +0x04
    };
    struct CResourcePage {
        long ScratchPool;                               // +0x00
        CArray<long> ResourceList;                      // +0x04
        unsigned char* NextFreeBufferAddress;           // +0x10
        long FreeSpaceSize;                             // +0x14
        bool EnableAutoRestore;                         // +0x18
    };
    virtual ~CGPUScratchBufferTextureManager();
    long AllocteMemoryPage(long scratchPool, bool enableAutoRestore);
private:
    CArray<CResourceEntry> Resources;                   // +0x04 (CResourceEntry is incomplete here: not read)
    CArray<CResourcePage> MemoryPages;                  // +0x10
    CArray<CScratchPool> ScratchPools;                  // +0x1c
    CArray<long> FreeTexturePageSlots;                  // +0x28
};

long CGPUScratchBufferTextureManager::AllocteMemoryPage(long scratchPool, bool enableAutoRestore)
{
    CScratchPool& pool = ScratchPools[scratchPool];
    CResourcePage page;
    page.ScratchPool = scratchPool;
    page.NextFreeBufferAddress = pool.Buffer;
    page.FreeSpaceSize = pool.BufferSize;
    page.EnableAutoRestore = enableAutoRestore;
    if (FreeTexturePageSlots.size() > 0) {
        long index = FreeTexturePageSlots[FreeTexturePageSlots.size() - 1];
        FreeTexturePageSlots.pop_back();
        MemoryPages[index] = page;
        return index;
    }
    long index = MemoryPages.size();
    MemoryPages.push_back(page);
    return index;
}