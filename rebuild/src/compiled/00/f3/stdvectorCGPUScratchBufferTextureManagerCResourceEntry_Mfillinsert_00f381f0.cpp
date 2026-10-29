// std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_fill_insert(iterator position, size_type n, const CResourceEntry& x) -- STLport fill insert of the 44-byte scratch-buffer resource entries: with room, copy x, shift the tail up (uninitialized copy + backward copy) and fill, or fill past the end and move the tail; without room, grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@UCResourceEntry@CGPUScratchBufferTextureManager@@V?$allocator@UCResourceEntry@CGPUScratchBufferTextureManager@@@std@@@std@@QAEXPAUCResourceEntry@CGPUScratchBufferTextureManager@@IABU34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F381F0, 282 bytes, thiscall, `ret 0Ch`.
// Identity (proven): reloc_bindings binds this exact symbol from the landed vector<CResourceEntry>::resize 0x00F385A0 (call at
//   0x00F385F8; the other reference is the insert jump at 0x00F38400); 44-byte stride (0x2E8BA2E9, `sar edx,3`); the x copy is the
//   CResourceEntry copy constructor 0x00F37970 (landed so) and the capacity arm calls _M_insert_overflow 0x00F37F60. STLport 4.6
//   public member (QAE). Catalog "Vector_Resize_CPreTransposedBoneMatrix" is wrong (that class is 48 bytes).
// Callees: CResourceEntry::CResourceEntry(const CResourceEntry&) 0x00F37970, __uninitialized_copy 0x00F37CC0, __copy_backward_ptrs
//   0x00F37D30, fill 0x00F37A00, __uninitialized_fill_n 0x00F37AD0 (all __fastcall), _M_insert_overflow 0x00F37F60.
// Shape: STLport 4.6 vector.c _M_fill_insert verbatim, with uninitialized_fill_n inlined onto the out-of-line
//   __uninitialized_fill_n. The tag of the first helper call is pushed once before the `elems_after > n` branch at the address of the
//   `position` argument slot (live in the else arm): __false_type has a user constructor and the tag-taking helpers have same-TU
//   `__declspec(noinline)` bodies. The x copy constructor must be opaque here (declared only): with a visible inline body VC7.1
//   still calls it but keeps _M_finish in a register across the call (280v282); retail reloads it.
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
        CResourceEntry(const CResourceEntry& other);              // 0x00F37970
    };
};
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { };
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
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
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
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&)   // 0x00F37D30
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);       // 0x00F37A00

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x00F37F60
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_fill_insert(CGPUScratchBufferTextureManager::CResourceEntry*, unsigned int, const CGPUScratchBufferTextureManager::CResourceEntry&);
}   // namespace std