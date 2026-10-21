// std::uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&) -- copy-construct n strings from x, return the end
// Leaf: ??$uninitialized_fill_n
// Retail: Fable.exe @ 0x00433504, 38 bytes, __fastcall (first in ecx, n in edx, &x on the stack, `ret 4`), returns
// first + n in eax. Catalog label std::_Fill_n<unsigned long*, unsigned int, unsigned long> is wrong: each element is
// built by the CCharString copy ctor (0x0099EC30) behind a placement-new null test, i.e. STLport
// uninitialized_fill_n -> __uninitialized_fill_n(.., __false_type) with the non-POD loop inlined. Caller 0x0043347D.
// Opt: size TU (unaligned entry; `push dword ptr [esp+0Ch]` re-read per element instead of /O2's ebx copy). Shape: the
// __false_type helper is marked __forceinline -- the size TU otherwise keeps it out of line (the retail body has it
// inlined into the 3-argument function, `ret 4`).
#pragma optimize("s", on)
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
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
template CCharString* __fastcall uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&);
}