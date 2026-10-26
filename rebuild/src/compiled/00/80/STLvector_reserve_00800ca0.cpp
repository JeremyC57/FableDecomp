// std::vector::reserve(size_type n) -- STLport vector<pair<C2DVector, unsigned char> >: grow the route-point storage to at least n elements, copying the old points into a fresh block and freeing the old one
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800CA0, 141 bytes (+ int3 padding to 144), __fastcall, `ret 4`.
//   Caller: CNavigatorAStar::CalculateRouteFromTo 0x00A1D5D0 (`route.reserve(Route.size())`, landed).
// Name: STLport 4.x `vector::reserve`; the catalog's "std_vector_Reserve" is the leaf.
// Callees: vector::_M_allocate_and_copy(n, first, last) 0x00800380 (this chunk), MSVCR71 malloc 0x00BFEA0E / free
//   0x00BFEA14 (the retail STLport allocator).
// Layout: 12-byte STLport vector (_M_start, _M_finish, _M_end_of_storage) of 9-byte pack(1) pair<C2DVector, unsigned char>.
// Shape: STLport 4.x `if (capacity() < n) { old = size(); if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start,
//   _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old, tmp + n); }` -- capacity() and size()
//   are the signed pointer differences (reciprocal multiply by 9) compared unsigned; _M_clear's destroy is trivial and
//   its deallocate is `if (p) free(p)`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

namespace std {
#pragma pack(push, 1)
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
#pragma pack(pop)

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    static _Tp* allocate(unsigned long __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    static void deallocate(_Tp* __p) { if (__p) free(__p); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    _Tp* _M_allocate_and_copy(unsigned long __n, const _Tp* __first, const _Tp* __last);   // 0x00800380
    void _M_clear() { deallocate(_M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage = __e;
    }
    void reserve(unsigned long __n);
};

template <class _Tp>
void vector<_Tp>::reserve(unsigned long __n)
{
    if (capacity() < __n) {
        const unsigned long __old_size = size();
        _Tp* __tmp;
        if (_M_start) {
            __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _M_clear();
        } else {
            __tmp = allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template class vector<pair<C2DVector, unsigned char> >;
}