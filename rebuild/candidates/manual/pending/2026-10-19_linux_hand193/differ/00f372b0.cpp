// CGPUScratchBufferTextureManager::AllocResourceEntry(long page) -- take a free resource slot (or append a default
// Grade: DIFFER(ours v retail) -- 229v224: retail keeps the address-taken index in esi as well as its frame home (this in edi, zero in ebx); our build keeps it only in the frame (this in esi, zero in edi) and reloads it for every use
// entry), bind it to the memory page and list it on that page; returns the resource index
// Leaf: AllocResourceEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F372B0, length 224 bytes, `ret 4`.
// Callees: vector<CResourceEntry>::resize(size_type, const CResourceEntry&) @ 0x00F385A0, the STLport
// vector<long>::_M_insert_overflow @ 0x00440990 (an ICF-shared body). Graphics layer.
// Retail layout (PDB names; PDB CArrays 16 bytes, retail STLport vectors 12): Resources +4 (0x2C-byte CResourceEntry:
// ResourceType, Texture +4, Surface +8, PageIndex +0xC, Dimensions, NoLevels, PixelFormat +0x1C, MultiSampleType,
// UseageFlags, MemoryPool), MemoryPages +0x10 (0x1C-byte CResourcePage: ScratchPool, ResourceList vector +4,
// NextFreeBufferAddress, FreeSpaceSize, EnableAutoRestore), FreeTextureSlots +0x34.
// Shape: the free-slot stack is read as `*(begin() + size() - 1)` then pop_back; the default entry temporary has an
// inline ctor (zero type/texture/surface, page and pixel format -1); the index local is address-taken by the page
// list's inline push_back (overflow helper with the __true_type tag in the dead `page` slot).

#include "engine/CPixelFormat.h"

struct CTexture;
struct CSurface;
struct C2DExtentsI { long Width; long Height; };

struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; }
    void resize(unsigned long newSize, const T& value);     // 0x00F385A0 (vector<CResourceEntry>)
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00440990 (vector<long>)
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CPixelFormatDefault : CPixelFormat {
    CPixelFormatDefault() { FormatIndex = -1; }
};

class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry {
        long ResourceType;                          // +0x00
        CTexture* Texture;                          // +0x04
        CSurface* Surface;                          // +0x08
        long PageIndex;                             // +0x0C
        C2DExtentsI Dimensions;                     // +0x10
        long NoLevels;                              // +0x18
        CPixelFormatDefault PixelFormat;            // +0x1C
        long MultiSampleType;                       // +0x20
        unsigned long UseageFlags;                  // +0x24
        long MemoryPool;                            // +0x28
        CResourceEntry() : ResourceType(0), Texture(0), Surface(0), PageIndex(-1) {}
    };
    struct CResourcePage {
        long ScratchPool;                           // +0x00
        CRetailVector<long> ResourceList;           // +0x04
        unsigned char* NextFreeBufferAddress;       // +0x10
        long FreeSpaceSize;                         // +0x14
        bool EnableAutoRestore;                     // +0x18
    };

    virtual ~CGPUScratchBufferTextureManager();
    CRetailVector<CResourceEntry> Resources;        // +0x04
    CRetailVector<CResourcePage> MemoryPages;       // +0x10
    unsigned char _pad1c[0x18];
    CRetailVector<long> FreeTextureSlots;           // +0x34
    long AllocResourceEntry(long page);
};

long CGPUScratchBufferTextureManager::AllocResourceEntry(long page)
{
    long index;
    if (FreeTextureSlots.size()) {
        index = FreeTextureSlots[FreeTextureSlots.size() - 1];
        FreeTextureSlots.pop_back();
    } else {
        index = Resources.size();
        Resources.resize(index + 1, CResourceEntry());
    }
    CResourceEntry& entry = Resources[index];
    entry.PageIndex = page;
    entry.Surface = 0;
    entry.Texture = 0;
    MemoryPages[page].ResourceList.push_back(index);
    return index;
}
