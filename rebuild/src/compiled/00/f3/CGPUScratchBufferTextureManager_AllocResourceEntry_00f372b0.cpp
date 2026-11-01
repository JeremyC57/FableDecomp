// CGPUScratchBufferTextureManager::AllocResourceEntry(long page) -- take a free resource slot (or append a default
// entry), bind it to the memory page and list it on that page; returns the resource index
// Leaf: AllocResourceEntry
// Naming wave 27 (family vl2, DECISIONS_W26 D6): the members are CArray<T> (Ego_r.pdb) over STLport std::vector<T, allocator<T> >:
//   vector<CResourceEntry>::resize(size_type unsigned int, const T&) 0x00F385A0 and the protected vector<long>::_M_insert_overflow
//   0x00440990 take their own rows' spellings. Was a row-local CRetailVector<T> (?resize@?$CRetailVector@.. / ?_M_insert_overflow@?$CRetailVector@J..K_N).
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

class CTexture;
class CSurface;
class C2DExtentsI { public: long Width; long Height; };

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() {} };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    _Tp& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; }
    void resize(unsigned int newSize, const _Tp& value);    // 0x00F385A0 (vector<CResourceEntry>)
protected:
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fillLength, bool atEnd);   // 0x00440990 (vector<long>)
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray (Ego_r.pdb member type)
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T, class _Alloc>
__declspec(noinline) void std::vector<T, _Alloc>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned int fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)

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
        CArray<long> ResourceList;           // +0x04
        unsigned char* NextFreeBufferAddress;       // +0x10
        long FreeSpaceSize;                         // +0x14
        bool EnableAutoRestore;                     // +0x18
    };

    virtual ~CGPUScratchBufferTextureManager();
    CArray<CResourceEntry> Resources;        // +0x04
    CArray<CResourcePage> MemoryPages;       // +0x10
    unsigned char _pad1c[0x18];
    CArray<long> FreeTextureSlots;           // +0x34
    long AllocResourceEntry(long page);
};

long CGPUScratchBufferTextureManager::AllocResourceEntry(long page)
{
    long index;
    long listIndex;
    if (FreeTextureSlots.size()) {
        index = FreeTextureSlots[FreeTextureSlots.size() - 1];
        listIndex = index;
        FreeTextureSlots.pop_back();
    } else {
        index = Resources.size();
        listIndex = index;
        Resources.resize(index + 1, CResourceEntry());
    }
    CResourceEntry& entry = Resources[index];
    entry.PageIndex = page;
    entry.Surface = 0;
    entry.Texture = 0;
    MemoryPages[page].ResourceList.push_back(listIndex);
    return index;
}