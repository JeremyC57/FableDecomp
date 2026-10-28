// CGPUScratchBufferTextureManager::FreeResourceEntry(long index) -- return a scratch resource slot: push it on FreeTextureSlots, clear the entry (delete its texture and surface), then erase the index from its memory page's resource list
// Leaf: ?FreeResourceEntry@CGPUScratchBufferTextureManager@@AAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36F40, 233 bytes (+ int3 padding), `ret 4`.
// Name: Ego_r.pdb ?FreeResourceEntry@CGPUScratchBufferTextureManager@@AAEXJ@Z (Ego_r 0x00B3C3D0, private); catalog name
//   "TexturePool_FreeSlot", landed caller 0x00F37030 FreeMemoryPage calls it "FreeTextureSlot".
// Callees: vector<long>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00440990, CTexture::Uninitialise @ 0x009F9F70
//   (CTexture has no out-of-line dtor in the PDB: its inline dtor is the Uninitialise call), operator delete @ 0x00BFE9BC,
//   CSurface virtual deleting dtor (vtable slot 0), memmove @ 0x00BFEAE6 (STLport __copy_trivial inside erase).
// Layout: Ego_r.pdb CGPUScratchBufferTextureManager (88 bytes there) with retail 12-byte STLport vectors: Resources +0x04,
//   MemoryPages +0x10, ScratchPools +0x1c, FreeTexturePageSlots +0x28, FreeTextureSlots +0x34, ShutdownFlag +0x40
//   (as the landed FreeMemoryPage 0x00F37030). CResourceEntry is 0x2c bytes (PDB 44), CResourcePage 0x1c (PDB 32, CArray 12).
// Shape: erase(position) is STLport 4.6 `if (pos + 1 != end()) __copy_trivial(pos + 1, _M_finish, pos)` with the real
//   __copy_trivial (`(__last == __first) ? __result : memmove(...) + n`): its own last==first test is the surviving
//   `cmp eax,ecx` (finish first); a __copy_trivial without that test leaves erase's `cmp ecx,eax`.
// Local models: there is no engine header for CGPUScratchBufferTextureManager; engine/CTexture.h and engine/CSurface.h are
//   flat data layouts with no member functions, so the delete/Uninitialise/virtual-dtor callees need local classes.
#include <new>
#include <string.h>
#include <stddef.h>

namespace std {

struct __true_type { __true_type() { } };

template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
          + ((const char*)__last - (const char*)__first);
}

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef T* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    T& operator[](size_t n) { return *(begin() + n); }
    protected:
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __true_type&, size_t fillLen, bool atEnd = false);   // 0x00440990
    public:
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __true_type(), 1UL, true);
    }
    iterator erase(iterator position)
    {
        if (position + 1 != end())
            __copy_trivial(position + 1, this->_M_finish, position);
        --this->_M_finish;
        return position;
    }
};

// Stand-in body for the out-of-line grow (retail 0x00440990 is this template instantiated in the same TU). Only the leaf
// above is graded. VC7.1 analyses same-TU noinline bodies: one that only reads `x` and leaves the tag unnamed lets the
// caller home the empty tag on the live `index` argument slot and keep `index` unaliased afterwards, as retail does
// (the landed SetItemAsPickedUp 0x007CB850 and FreeMemoryPage 0x00F37030 use the same stand-in device).
template <class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __true_type&, size_t fillLen, bool atEnd)
{
    if (x == 0 && atEnd)
        this->_M_finish = position + fillLen;
}

}   // namespace std

class CTexture {
public:
    void* PD3DTexture;                       // +0x00
    unsigned long Bits;                      // +0x04 ByteLength:28 / AllocationSource:4
    void Uninitialise();                     // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};

class CSurface {
public:
    virtual ~CSurface();                     // vtable slot 0 (scalar deleting dtor)
};

class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry {                  // 0x2c bytes
        long          ResourceType;          // +0x00 EResourceType
        CTexture*     Texture;               // +0x04
        CSurface*     Surface;               // +0x08
        long          PageIndex;             // +0x0c
        long          Dimensions[2];         // +0x10 C2DExtentsI
        long          NoLevels;              // +0x18
        unsigned long PixelFormat;           // +0x1c CPixelFormat
        long          MultiSampleType;       // +0x20
        unsigned long UseageFlags;           // +0x24
        long          MemoryPool;            // +0x28
    };
    struct CResourcePage {                   // 0x1c bytes
        long                ScratchPool;           // +0x00
        std::vector<long>  ResourceList;          // +0x04
        unsigned char*      NextFreeBufferAddress; // +0x10
        long                FreeSpaceSize;         // +0x14
        bool                EnableAutoRestore;     // +0x18
    };
    struct CScratchPool { unsigned char* Buffer; long BufferSize; };

    virtual ~CGPUScratchBufferTextureManager();
    std::vector<CResourceEntry> Resources;            // +0x04
    std::vector<CResourcePage>  MemoryPages;          // +0x10
    std::vector<CScratchPool>   ScratchPools;         // +0x1c
    std::vector<long>           FreeTexturePageSlots; // +0x28
    std::vector<long>           FreeTextureSlots;     // +0x34
    bool                         ShutdownFlag;         // +0x40

private:
    void FreeResourceEntry(long index);
};

void CGPUScratchBufferTextureManager::FreeResourceEntry(long index)
{
    FreeTextureSlots.push_back(index);
    long pageIndex = Resources[index].PageIndex;
    Resources[index].ResourceType = 0;
    delete Resources[index].Texture;
    delete Resources[index].Surface;
    Resources[index].Texture = 0;
    Resources[index].Surface = 0;
    CResourcePage& page = MemoryPages[pageIndex];
    for (unsigned long i = 0; i < page.ResourceList.size(); ++i) {
        if (page.ResourceList[i] == index) {
            page.ResourceList.erase(page.ResourceList.begin() + i);
            return;
        }
    }
}