// std::vector<pair<C3DVector,C3DVector> >::_M_insert_overflow(pair* pos, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport reallocating insert of 24-byte (point, point) pairs: allocate old_size + max(old_size, n), copy the head, construct the n new pairs, copy the tail unless appending, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007700D0, 238 bytes (+ int3 padding), thiscall `ret 14h`.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (STLport's malloc allocator: `n != 0 ? malloc(n * 24) : 0`,
//   `if (p) free(p)`), __uninitialized_copy<pair*> @ 0x0076F780 and __uninitialized_fill_n<pair*> @ 0x0076F7D0 (both
//   __fastcall, `ret 8`).
// Caller: CTCWeaponTrail::GetLastWeaponTrailPoints 0x0076E4D0 (Ego_r.pdb: `vector<pair<C3DVector,C3DVector> >&` out
//   parameter, filled by push_back from the trail list), so the element is std::pair<C3DVector,C3DVector> (24 bytes).
// Label correction: catalogued as `Vector_Insert_CAnimatedBlock_MoveAndRealloc`; the body is STLport
//   `vector<pair<C3DVector,C3DVector> >::_M_insert_overflow` (non-POD path: C3DVector has a user copy constructor).
// Shape: STLport 4.x verbatim (the landed 0x004ADB50 model): the `__false_type()` tag temporaries are homed on the bool
//   `__atend` argument slot, which needs same-TU `auto_inline(off)` stand-ins of the tag-taking COMDATs; `max` by
//   reference with old_size in the `push ecx` frame word; the n == 1 placement copy is memberwise (six in-order dword
//   moves); _M_clear() is inline and reduces to the deallocate (the pair's destructor is trivial).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };              // user constructor: no zero-initialised tag temporary

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

typedef pair<C3DVector, C3DVector> CPointPair;

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x0076F780 / 0x0076F7D0 (they never touch the tag).
CPointPair* __fastcall __uninitialized_copy(CPointPair* __first, CPointPair* __last, CPointPair* __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        new (__result) CPointPair(*__first);
    return __result;
}
CPointPair* __fastcall __uninitialized_fill_n(CPointPair* __first, unsigned int __n, const CPointPair& __x, const __false_type&)
{
    CPointPair* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (__cur) CPointPair(__x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp>
struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
inline void _Destroy(CPointPair* __first, CPointPair* __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(__first);
}

struct CPointPairVector {
    CPointPair* _M_start;                            // +0x00
    CPointPair* _M_finish;                           // +0x04
    _STLP_alloc_proxy<CPointPair*, CPointPair, allocator<CPointPair> > _M_end_of_storage;   // +0x08

    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(CPointPair* __s, CPointPair* __f, CPointPair* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(CPointPair* __position, const CPointPair& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);
};

void CPointPairVector::_M_insert_overflow(CPointPair* __position, const CPointPair& __x, const __false_type&,
                                          unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    CPointPair* __new_start = _M_end_of_storage.allocate(__len);
    CPointPair* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        new (__new_finish) CPointPair(__x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}