// std::__pop_heap_aux(pair<EExpressionType,CExpressionSet::CEntry>* first, last, pair<..>*, CKeyPairCompareLess<..>) -- pop the heap top into the last slot and re-heap [first, last - 1)
// Leaf: ??$__pop_heap_aux@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@U12@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@std@@YIXPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@00V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@Z
// Flags: /Gr
// Retail: Fable.exe @ 0x0086F9A0, 86 bytes, __fastcall (first ecx, last edx; the value-type pointer and the comparator on
// the stack), `ret 8`.
// Identity (proven by shape and callee): STLport 4.6 `__pop_heap_aux(first, last, _Tp*, comp)` =
// `__pop_heap(first, last - 1, last - 1, _Tp(*(last - 1)), comp, (int*)0)` with __pop_heap inline: the 12-byte value
// *(last - 1) saved in esi/edi/ebx, *first copied over it, then __adjust_heap(first, 0, (last - 1) - first, value, comp).
// The callee 0x0086F450 is bound as ??$__adjust_heap@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@HU12@...
// (landed, element pair<EExpressionType, CExpressionSet::CEntry>), and the bound __make_heap of the same element is the
// neighbouring 0x0086F930; the caller 0x0086FA87 is that sort's heap loop.
// Label correction: catalog "_gapscan::sub_0086f9a0".
// Callees: 0x0086F450 std::__adjust_heap<pair<EExpressionType, CExpressionSet::CEntry>*, int, pair<..>, CKeyPairCompareLess<..> >.
// Layout: Ego_r.pdb CExpressionSet::CEntry = { Type +0, AnimIndex +4 } (8 bytes), the key EExpressionType +0 of the pair.
// Shape: the pair's user copy constructor builds the by-value argument word by word through `mov eax,esp`.
enum EExpressionType { EXPRESSION_TYPE_NONE };

class CExpressionSet {
public:
    class CEntry {
    public:
        long Type;                                          // +0x0
        long AnimIndex;                                     // +0x4
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<EExpressionType, CExpressionSet::CEntry> TExprEntry;
typedef CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> TExprLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <> void __adjust_heap(TExprEntry* __first, int __holeIndex, int __len, TExprEntry __val, TExprLess __comp);   // 0x0086F450

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                       _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template void __pop_heap_aux(TExprEntry*, TExprEntry*, TExprEntry*, TExprLess);
}