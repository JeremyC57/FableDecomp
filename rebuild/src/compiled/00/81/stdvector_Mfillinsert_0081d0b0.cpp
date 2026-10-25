// std::vector::_M_fill_insert(CTextureMorphEntry* position, unsigned int n, const CTextureMorphEntry& x) -- STLport vector insert(pos, n, x): with room, shift the tail up (uninitialized copy of the last n, or a fill of the overhang) and fill the gap from a local copy of x; without room, regrow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081D0B0, 336 bytes, thiscall `ret 0Ch`.
// Identity: a vector of the 20-byte CTextureMorphEntry (Ego_r.pdb; CTextureMorphs::GetAllMorphsOfType 0x0081B0A0 fills
//   one): reached from resize(n, x) 0x0081DAA0 (0x0081DAEE) and the jump thunk 0x0081D750.
//   The catalog's "std_vector_Insert_Helper_0x14" is descriptive.
// Callees (all out of line, __fastcall / thiscall): __uninitialized_copy 0x0081A650, __uninitialized_fill_n 0x008195E0,
//   __copy_backward 0x008197B0, fill 0x00819320, _M_insert_overflow 0x0081C270.
// Layout: CTextureMorphEntry = 20 bytes (0x66666667, `sar 3`); x_copy is a five-word frame copy.
// Shape: STLport 4.6 `_M_fill_insert` verbatim (as the 0x007D24F0 instantiation): every empty tag and both spills
//   (`old_finish - n`, `n - elems_after`) share the dead x argument slot, which needs the five out-of-line helpers as
//   same-TU `auto_inline(off)` + noinline template bodies (their real STLport text).
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };

class CTextureMorphEntry {                           // Ego_r.pdb, 20 bytes, trivially copyable
public:
    EHeroMorphType      MorphType;                   // +0x00
    long                TextureLayer;                // +0x04
    long                ReplacingTextureIndex;       // +0x08
    long                BankIndex;                   // +0x0c
    ECompositeBlendType Blend;                       // +0x10
};

inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator, _ForwardIterator) { }     // trivially destructible element: no loop

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0081A650
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                        // 0x008195E0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x008197B0
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)          // 0x00819320
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
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
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
#pragma auto_inline(off)
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                 size_type __fill_len, bool __atend = false)             // 0x0081C270
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
#pragma auto_inline(on)
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
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

typedef CTextureMorphEntry TElem;
template void vector<TElem>::_M_fill_insert(TElem*, unsigned int, const TElem&);
}