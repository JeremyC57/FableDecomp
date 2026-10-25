// std::vector::_M_allocate_and_copy(size_type n, const_pointer first, const_pointer last) -- STLport vector<pair<C2DVector, unsigned char> >: allocate room for n route points and copy-construct [first, last) into it; returns the new block
// Leaf: ?_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800380, 89 bytes (+ int3 padding), __fastcall (`this` unused), `ret 0Ch`.
//   Caller: vector<pair<C2DVector, unsigned char> >::reserve 0x00800CA0 (this chunk), the route vector of
//   CTCPreCalculatedNavigationRoute (Ego_r.pdb `Route`) and CNavigatorAStar::CalculateRouteFromTo 0x00A1D5D0.
// Name: STLport 4.x member (the donor's Dinkumware build has no counterpart); the catalog's
//   "std_vector_Allocate_9ByteElements" is descriptive.
// Callees: MSVCR71 malloc 0x00BFEA0E (the retail STLport allocator is malloc/free).
// Layout: 9-byte pair<C2DVector, unsigned char> (pack(1), the landed 0x008002F0 / 0x00A1D5D0 model; C2DVector has the
//   memberwise user copy constructor).
// Shape: `allocate(n)` inlined (`n != 0 ? malloc(n * 9) : 0`), then STLport's `__uninitialized_copy(first, last,
//   result, __false_type())` inlined: a placement-new (null-tested) memberwise copy per element; the untouched block
//   pointer is returned.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct C2DVector {
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

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (&*__cur) pair<C2DVector, unsigned char>(*__first);
    return __cur;
}

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    static _Tp* allocate(unsigned long __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    _Tp* _M_allocate_and_copy(unsigned long __n, const _Tp* __first, const _Tp* __last);
};

template <class _Tp>
_Tp* vector<_Tp>::_M_allocate_and_copy(unsigned long __n, const _Tp* __first, const _Tp* __last)
{
    _Tp* __result = allocate(__n);
    __uninitialized_copy(__first, __last, __result, __false_type());
    return __result;
}

template class vector<pair<C2DVector, unsigned char> >;
}