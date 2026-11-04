// CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager() -- free every memory page still bound to a scratch pool (FreeMemoryPage(i) while ScratchPool > -1), then the five member arrays (FreeTextureSlots, FreeTexturePageSlots, ScratchPools, MemoryPages with each page's ResourceList, Resources), then the CBaseClassNonCopyable base
// Leaf: ??1CGPUScratchBufferTextureManager
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F37570, 225 bytes. Callers: the ??_G 0x00F388D0 (slot 0 of the vtable 0x01301CE4) and
//   0x00B3B984 (an owner's destructor destroying the manager embedded at its +0x3190).
// Identity: first store = vtable 0x01301CE4 (RTTI .?AVCGPUScratchBufferTextureManager@@ : CBaseClassNonCopyable : CBaseClass);
//   Ego_r.pdb ??1CGPUScratchBufferTextureManager@@UAE@XZ (user-declared virtual: the store stays).
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage(long) 0x00F37030 (Ego_r.pdb ?FreeMemoryPage@CGPUScratchBufferTexture
//   Manager@@QAEXJ@Z: its body writes -1 to MemoryPages[i].ScratchPool); free 0x00BFEA14 (STLport deallocate);
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Layout (Ego_r.pdb, 12-byte retail CArray): Resources +0x04, MemoryPages +0x10 (CResourcePage 0x1C: ScratchPool +0,
//   ResourceList CArray<long> +4, NextFreeBufferAddress, FreeSpaceSize, EnableAutoRestore), ScratchPools +0x1C,
//   FreeTexturePageSlots +0x28, FreeTextureSlots +0x34, ShutdownFlag +0x40.
// Shape: a signed `long i` against the unsigned size() (`jb`, the /0x1C magic recomputed each pass), the test written
//   `> -1` (`cmp [..],-1 ; jle`; `!= -1` gives `je`); members then die in reverse order, the CResourcePage elements through
//   their implicit dtor (ResourceList's free).
// Label: manifest "_global::CGPUScratchBufferTextureManager_Destructor" was descriptive; the PDB name is the class dtor.
// Model: no engine header for the class; CArray<T> is the engine's empty wrapper over the STLport vector.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
#include "engine/CArray.h"
#include "engine/CBaseClassNonCopyable.h"
class CTexture;
class CSurface;
class CGPUScratchBufferTextureManager : public CBaseClassNonCopyable {
public:
    struct CResourceEntry {                                              // 0x2c bytes (Ego_r.pdb; no containers)
        long ResourceType;                                               // EResourceType
        CTexture* Texture;
        CSurface* Surface;
        long PageIndex;
        long Dimensions[2];                                              // C2DExtentsI
        long NoLevels;
        long PixelFormat;                                                // CPixelFormat
        long MultiSampleType;                                            // ESurfaceMultisampleType
        unsigned long UseageFlags;
        long MemoryPool;                                                 // ESurfacePool
    };
    struct CResourcePage {                                               // 0x1c bytes
        long ScratchPool;                                                // +0x00
        CArray<long> ResourceList;                                       // +0x04
        unsigned char* NextFreeBufferAddress;                            // +0x10
        long FreeSpaceSize;                                              // +0x14
        bool EnableAutoRestore;                                          // +0x18
    };
    struct CScratchPool {                                                // 8 bytes
        unsigned char* Buffer;
        long BufferSize;
    };
    CGPUScratchBufferTextureManager();                                   // retail ctor stores the vtable at 0x00F3752C
    virtual ~CGPUScratchBufferTextureManager();
    void FreeMemoryPage(long index);                                     // 0x00F37030
    CArray<CResourceEntry> Resources;                                    // +0x04
    CArray<CResourcePage> MemoryPages;                                   // +0x10
    CArray<CScratchPool> ScratchPools;                                   // +0x1c
    CArray<long> FreeTexturePageSlots;                                   // +0x28
    CArray<long> FreeTextureSlots;                                       // +0x34
    bool ShutdownFlag;                                                   // +0x40
};
CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager()
{
    for (long i = 0; i < MemoryPages.size(); ++i) {
        if (MemoryPages[i].ScratchPool > -1)
            FreeMemoryPage(i);
    }
}