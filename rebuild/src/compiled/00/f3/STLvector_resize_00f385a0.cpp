// std::vector::resize(size_type new_size, const CResourceEntry& x) -- shrink with erase or grow with a fill insert (44-byte scratch-buffer resource entries)
// Leaf: ?resize@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F385A0, 99 bytes, thiscall, `ret 8`.
// Identity: vector<CGPUScratchBufferTextureManager::CResourceEntry>: callers CGPUScratchBufferTextureManager::
//   AllocResourceEntry (0x00F37326) and the one-argument resize 0x00F38640; divide by 44 (0x2E8BA2E9, `sar edx,3`) =
//   Ego_r.pdb sizeof(CResourceEntry) (ResourceType, Texture, Surface, PageIndex, Dimensions, NoLevels, PixelFormat,
//   MultiSampleType, UseageFlags, MemoryPool). Catalog "vector<CVectorMap<unsigned long,...>>::resize" is propagated.
// Callees: erase(iterator, iterator) @ 0x00F37EE0, _M_fill_insert @ 0x00F381F0.
// Shape: STLport 4.6 `resize(n, x)` (see 0x0052BEA0); 44 gives `imul esi,esi,2Ch`.
class CTexture;
class CSurface;

class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry {                   // 44 bytes (FableWin.pdb: ??4CResourcePage@...QAEAAU01@ -- the nested records are structs)
        long ResourceType;                    // +0x00  EResourceType
        CTexture* Texture;                    // +0x04
        CSurface* Surface;                    // +0x08
        long PageIndex;                       // +0x0C
        long DimensionsX, DimensionsY;        // +0x10  C2DExtentsI
        long NoLevels;                        // +0x18
        unsigned long PixelFormat;            // +0x1C  CPixelFormat
        long MultiSampleType;                 // +0x20  ESurfaceMultisampleType
        unsigned long UseageFlags;            // +0x24
        long MemoryPool;                      // +0x28  ESurfacePool
    };
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                     // out of line in retail
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);    // out of line in retail
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x) {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<CGPUScratchBufferTextureManager::CResourceEntry>::resize(size_type, const CGPUScratchBufferTextureManager::CResourceEntry&);
}