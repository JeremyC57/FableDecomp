// std::__uninitialized_fill_n(pair<CCharString,CCharString>* first, unsigned int n, const pair<CCharString,CCharString>& x, const __false_type&) -- STLport placement fill of n raw (string, string) pair slots with copies of x, returning the end of the filled range
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076F740, 54 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx -> esi,
//   n in edx -> edi, &x and the empty tag on the stack: `ret 8`).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice per element: first at +0, second at +4).
// Caller: vector<pair<CCharString,CCharString> >::_M_insert_overflow @ 0x007704A0 (the fill_len != 1 arm), so the element
//   is the 8-byte pair of two CCharStrings. The catalog label `_Fill_n<unsigned long*,unsigned int,unsigned long>` is
//   propagated; the body is STLport `__uninitialized_fill_n` (non-POD path).
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- unsigned n gives `test edx,edx ;
//   jbe`, the placement new gives the `test esi,esi ; je` null test, and STLport's explicit pair copy constructor copies
//   `first` then `second` (`lea eax,[ebx+4]` re-derived each iteration).
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<CCharString, CCharString> CStringPair;

template CStringPair* __fastcall __uninitialized_fill_n<CStringPair*, unsigned int, CStringPair>(
    CStringPair*, unsigned int, const CStringPair&, const __false_type&);

}