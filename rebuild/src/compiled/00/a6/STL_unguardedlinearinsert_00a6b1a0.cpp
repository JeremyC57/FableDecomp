// _STL::__unguarded_linear_insert(pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long> val, less<pair<unsigned long,unsigned long> > comp) -- insertion-sort inner step: shift larger pairs up one slot until val's lexicographic place is found, no lower-bound test
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B1A0, 56 bytes (+8 int3), `ret 0Ch` (link-time __fastcall: last in
//   ecx, the 8-byte pair value and the empty comparator on the stack). No callees.
// Caller: 0x00A6B881, the else arm of the same instantiation's __linear_insert (0x00A6B820).
// Identity: the manifest's "InsertionSort_ShiftAndInsert_u64Pair" is descriptive. The TU is the CPhysicsMesh sort
//   family of std::pair<unsigned long, unsigned long> (catalogued _Push_heap 0x00A6B1E0 / _Adjust_heap 0x00A6B4F0 of
//   that pair, Ego_r.pdb names). The compare is the default less<pair>: STLport's pair operator<
//   `x.first < y.first || (!(y.first < x.first) && x.second < y.second)` -- unsigned `ja/jb` on the first word, then
//   `jae` on the second.
// Shape: STLport `__unguarded_linear_insert`: `next = last; --next; while (comp(val, *next)) { *last = *next; last =
//   next; --next; } *last = val;` -- val's two words live in esi/edi, next in eax, last in ecx.
namespace _STL {

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> class less {
public:
    less() { }
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

typedef pair<unsigned long, unsigned long> TPair;
template void __fastcall __unguarded_linear_insert<TPair*, TPair, less<TPair> >(TPair*, TPair, less<TPair>);

}