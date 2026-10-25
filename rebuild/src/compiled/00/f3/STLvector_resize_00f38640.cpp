// _STL::vector::resize(unsigned int new_size) -- STLport vector<CGPUScratchBufferTextureManager::CResourceEntry>::resize(n): resize the scratch manager's resource list to n entries, filling with an empty entry (no texture or surface, no page, no pixel format)
// Leaf: ?resize@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@V?$allocator@UCResourceEntry@CGPUScratchBufferTextureManager@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F38640, 47 bytes, thiscall on the vector, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?resize@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@...@@QAEXI@Z; no
//   direct call site in the image (xref: 0 calls / 0 references); its callee is the two-argument resize that
//   CGPUScratchBufferTextureManager::AllocResourceEntry 0x00F372B0 calls on Resources (CArray<CResourceEntry>).
// Callees: vector<CResourceEntry>::resize(n, const CResourceEntry&) 0x00F385A0 (out of line).
// Layout: Ego_r.pdb struct CGPUScratchBufferTextureManager::CResourceEntry (44 bytes): ResourceType +0, Texture +4,
//   Surface +8, PageIndex +0xC, Dimensions C2DExtentsI +0x10, NoLevels +0x18, PixelFormat +0x1C, MultiSampleType +0x20,
//   UseageFlags +0x24, MemoryPool +0x28; FableWin.pdb lists its default constructor. CPixelFormat() sets -1.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: one zero
//   register for the type / texture / surface, then `or eax,-1` for the page index and the pixel format; the rest is
//   left unset.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<CGPUScratchBufferTextureManager::CResourceEntry>::resize(n).
enum EResourceType { RESOURCE_TYPE_NONE = 0 };
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };
enum ESurfacePool { SURFACE_POOL_DEFAULT = 0 };
class CTexture;
class CSurface;

class C2DExtentsI {
public:
    long Width, Height;
    C2DExtentsI() { }
};

class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
};

class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry {                         // 44 bytes
        EResourceType           ResourceType;       // +0x00
        CTexture*               Texture;            // +0x04
        CSurface*               Surface;            // +0x08
        long                    PageIndex;          // +0x0C
        C2DExtentsI             Dimensions;         // +0x10
        long                    NoLevels;           // +0x18
        CPixelFormat            PixelFormat;        // +0x1C
        ESurfaceMultisampleType MultiSampleType;    // +0x20
        unsigned long           UseageFlags;        // +0x24
        ESurfacePool            MemoryPool;         // +0x28
        CResourceEntry() : ResourceType(RESOURCE_TYPE_NONE), Texture(0), Surface(0), PageIndex(-1) { }
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00F385A0
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template void _STL::vector<CGPUScratchBufferTextureManager::CResourceEntry>::resize(unsigned int);