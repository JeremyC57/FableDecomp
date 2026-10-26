// std::vector<pair<C2DVector,unsigned char> >::_M_insert_overflow(pair* position, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the 9-byte navigation route points: allocate old_size + max(old_size, fill_len), copy the head, construct the fill_len copies of x, copy the tail unless inserting at the end, free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800DA0, 262 bytes, thiscall `ret 14h`.
// Identity: callers 0x00800A39, 0x00800EE8 (the vector's insert/push_back front ends in this TU) and
//   CNavigatorAStar::CalculateRouteFromTo 0x00A1D5D0 (landed; its inline push_back calls this address as the overflow
//   path). The out-of-line tail copy 0x008002F0 is the landed __uninitialized_copy of the same 9-byte
//   pair<C2DVector, unsigned char> (Ego_r.pdb); the catalog's "std_vector_AllocateAndInsert" is descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x008002F0 (__fastcall,
//   `ret 8`), free 0x00BFEA14.
// Layout: pair<C2DVector, unsigned char>, pack(1), 9 bytes (first +0, second +8); C2DVector has the memberwise user copy
//   constructor (the landed 0x00800380 model), so every copy moves two dwords and a byte.
// Shape: STLport 4.6 `_M_insert_overflow` verbatim: `(max)(__old_size, __fill_len)` by reference (the old size gets the
//   `push ecx` frame word, the fill length its own slot), allocate inline (`n != 0 ? malloc(n * 9) : 0`, the block
//   homed on the dead fill_len slot), the head __uninitialized_copy inline, `if (__fill_len == 1) _Construct else
//   __uninitialized_fill_n` inline (`__forceinline`; plain `inline` leaves a cdecl call), the tail copy out of line
//   (VC7.1 inlines only the first of the two identical calls of the `__fastcall` inline template -- without
//   `__fastcall` the out-of-line call is cdecl), the trivial _Destroy folded away, `if (p) free(p)`, then _M_set.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

namespace std {
struct __false_type { __false_type() { } };

#pragma pack(push, 1)
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
#pragma pack(pop)

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
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
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
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

template void vector<pair<C2DVector, unsigned char> >::_M_insert_overflow(
    pair<C2DVector, unsigned char>*, const pair<C2DVector, unsigned char>&, const __false_type&, unsigned int, bool);
}