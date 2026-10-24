// _STL::__unguarded_linear_insert(SPair* last, SPair val, SLess comp) -- STLport insertion-sort inner step over 8-byte pair<long, unsigned long> records: shift every record that sorts after val up one slot, then store val in the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5AB0, 56 bytes (+8 int3 padding), `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: last in
//   ecx, the 8-byte val by value and the 4-byte empty comparator slot on the stack.
// Callers: __linear_insert 0x00AE6760 (the guarded step; it calls this body when val does not sort before *first).
// Element: pair<long, unsigned long> -- the same instantiation as the landed __adjust_heap 0x00AE5FB0 / sort_heap 0x00AE71A0:
//   STLport's pair operator< (`x.first < y.first || (!(y.first < x.first) && x.second < y.second)`) gives the signed
//   `jg/jl` on the first word, then an unsigned `jae` on the second.
// Label: catalog "InsertionSort_Insert64Bit" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
//   *last = val;`); val's two words stay in esi/edi for the whole loop.
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
}

typedef _STL::pair<long, unsigned long> SPair;
typedef _STL::less<SPair> SLess;

template void __fastcall _STL::__unguarded_linear_insert<SPair*, SPair, SLess>(SPair*, SPair, SLess);