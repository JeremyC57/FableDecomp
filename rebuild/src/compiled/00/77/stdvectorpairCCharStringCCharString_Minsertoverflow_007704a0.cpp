// std::vector<pair<CCharString,CCharString> >::_M_insert_overflow(pair* pos, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport reallocating insert of (string, string) pairs: allocate old_size + max(old_size, n), copy the head, construct the n new pairs, copy the tail unless appending, clear the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007704A0, 203 bytes, thiscall `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (STLport's malloc allocator: `n != 0 ? malloc(n * 8) : 0`),
//   __uninitialized_copy<pair*> @ 0x0076F700 (__fastcall first/last, result + tag on the stack),
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the n == 1 placement pair copy),
//   __uninitialized_fill_n<pair*> @ 0x0076F740 (this chunk), vector::_M_clear() @ 0x007701C0 (out of line: destroy + free).
// Caller: vector<pair<CCharString,CCharString> >::push_back 0x007706A0 (CTCWeaponTrail::HelperNames).
// Label correction: catalogued as `Vector_Insert_CCharString_Pair_MoveAndRealloc`; the body is STLport
//   `vector<pair<CCharString,CCharString> >::_M_insert_overflow` (non-POD path).
// Shape: STLport 4.x verbatim (the landed 0x004ADB50 model): every `__false_type()` tag temporary is homed on the bool
//   `__atend` argument slot (`lea ecx,[esp+28h]`), which VC7.1 does only when the tag-taking callees are visible and
//   never touch the tag (same-TU `auto_inline(off)` stand-ins for the COMDATs); `max` by reference with old_size in the
//   `push ecx` frame word; _M_clear() is a separate out-of-line member here (declared only).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };              // user constructor: no zero-initialised tag temporary

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

typedef pair<CCharString, CCharString> CStringPair;

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x0076F700 / 0x0076F740 (they never touch the tag).
CStringPair* __fastcall __uninitialized_copy(CStringPair* __first, CStringPair* __last, CStringPair* __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        new (__result) CStringPair(*__first);
    return __result;
}
CStringPair* __fastcall __uninitialized_fill_n(CStringPair* __first, unsigned int __n, const CStringPair& __x, const __false_type&)
{
    CStringPair* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (__cur) CStringPair(__x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp>
struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

struct CStringPairVector {
    CStringPair* _M_start;                           // +0x00
    CStringPair* _M_finish;                          // +0x04
    _STLP_alloc_proxy<CStringPair*, CStringPair, allocator<CStringPair> > _M_end_of_storage;   // +0x08

    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear();                                 // 0x007701C0
    void _M_set(CStringPair* __s, CStringPair* __f, CStringPair* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(CStringPair* __position, const CStringPair& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);
};

void CStringPairVector::_M_insert_overflow(CStringPair* __position, const CStringPair& __x, const __false_type&,
                                           unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    CStringPair* __new_start = _M_end_of_storage.allocate(__len);
    CStringPair* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        new (__new_finish) CStringPair(__x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}