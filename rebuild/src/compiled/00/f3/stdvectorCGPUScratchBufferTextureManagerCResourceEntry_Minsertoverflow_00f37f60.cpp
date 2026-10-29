// std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_insert_overflow(pointer position, const CResourceEntry& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth insert of the 44-byte scratch-buffer resource entries: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@V?$allocator@UCResourceEntry@CGPUScratchBufferTextureManager@@@std@@@std@@IAEXPAUCResourceEntry@CGPUScratchBufferTextureManager@@ABU34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F37F60, 265 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): element CGPUScratchBufferTextureManager::CResourceEntry (44 bytes: 0x2E8BA2E9 reciprocal with `sar edx,3`,
//   `imul eax,eax,2Ch`; Ego_r.pdb struct of ResourceType .. MemoryPool). The only caller is the vector's _M_fill_insert 0x00F381F0
//   (call at 0x00F382FD, its capacity-overflow arm), which the landed vector<CResourceEntry>::resize 0x00F385A0 binds; the helper
//   calls are the vector's own __uninitialized_copy 0x00F37CC0 and __uninitialized_fill_n 0x00F37AD0 (0x2C strides). STLport 4.6
//   protected member (IAE), the landed vector<CScratchPool> twin 0x00F380E0 right after it. Catalog "Vector_Reallocate_CameraTag" is wrong.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00F37CC0 and __uninitialized_fill_n 0x00F37AD0 (__fastcall:
//   first ecx, last / n edx; result or x and the tag reference on the stack).
// Shape: STLport 4.6 vector.c _M_insert_overflow. Retail passes the address of the live `atend` slot as the __false_type tag of all
//   three helper calls, which VC7.1 only does when the tag has a user constructor (no zero store) and the tag-taking helpers have
//   same-TU `__declspec(noinline)` bodies that never touch the tag (declared-only gave its own zeroed slot, 259v265). The
//   fill_len == 1 construct is the inline memberwise copy constructor (11 dword moves alternating edx/ecx, not `rep movsd`) behind
//   placement new's null test: CResourceEntry has user default and copy constructors in both PDB name sets, and `_Construct` is
//   `__forceinline` (plain `inline` left it an out-of-line call, 207v265). The trivial destructor loop of _M_clear vanishes.
class CTexture;
class CSurface;
enum EResourceType { EResourceType_Force32 = 0x7fffffff };
enum ESurfaceMultisampleType { ESurfaceMultisampleType_Force32 = 0x7fffffff };
enum ESurfacePool { ESurfacePool_Force32 = 0x7fffffff };
class C2DExtentsI { public: long X, Y; };
#include "engine/CPixelFormat.h"                       // FormatIndex, 4 bytes
class CGPUScratchBufferTextureManager {
public:
    struct CResourceEntry {
        EResourceType            ResourceType;                    // +0x00
        CTexture*                Texture;                         // +0x04
        CSurface*                Surface;                         // +0x08
        long                     PageIndex;                       // +0x0C
        C2DExtentsI              Dimensions;                      // +0x10
        long                     NoLevels;                        // +0x18
        CPixelFormat             PixelFormat;                     // +0x1C
        ESurfaceMultisampleType  MultiSampleType;                 // +0x20
        unsigned long            UseageFlags;                     // +0x24
        ESurfacePool             MemoryPool;                      // +0x28
        CResourceEntry();
        CResourceEntry(const CResourceEntry& other)
            : ResourceType(other.ResourceType), Texture(other.Texture), Surface(other.Surface),
              PageIndex(other.PageIndex), Dimensions(other.Dimensions), NoLevels(other.NoLevels),
              PixelFormat(other.PixelFormat), MultiSampleType(other.MultiSampleType),
              UseageFlags(other.UseageFlags), MemoryPool(other.MemoryPool)
        {
        }
    };
};
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
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
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00F37CC0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00F37AD0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_insert_overflow(CGPUScratchBufferTextureManager::CResourceEntry*, const CGPUScratchBufferTextureManager::CResourceEntry&, const __false_type&, unsigned int, bool);
}   // namespace std