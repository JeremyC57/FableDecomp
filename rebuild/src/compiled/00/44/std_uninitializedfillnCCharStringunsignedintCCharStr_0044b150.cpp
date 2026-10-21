// std::__uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&, const __false_type&) -- copy-construct n strings, return the end
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe @ 0x0044B150, 42 bytes, __fastcall (first in ecx, n in edx, &x and the __false_type tag on the stack,
// `ret 8`), returns first + n. Catalog label std::_Fill_n<unsigned long*, unsigned int, unsigned long> is wrong: each
// element is built by the CCharString copy ctor (0x0099EC30) behind a placement-new null test -- STLport's non-POD
// __uninitialized_fill_n, here out of line (the 3-argument wrapper 0x00433504 has it inlined). Caller 0x0057FD02.
// Shape: /O2 (x cached in ebx, `nop` loop-head pad).
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template CCharString* __fastcall __uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&, const __false_type&);
}