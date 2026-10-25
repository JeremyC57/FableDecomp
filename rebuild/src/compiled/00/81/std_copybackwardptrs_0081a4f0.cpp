// std::__copy_backward_ptrs(first, last, result, const __false_type&) -- (std::pair<EExpressionType, CExpressionSet::CEntry>) STLport backward copy of 40-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 10-dword `rep movsd`); returns result - n
// Leaf: ??$__copy_backward_ptrs@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@PAU12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081A4F0, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element std::pair<EExpressionType, CExpressionSet::CEntry> -- proven; evidence: its only caller 0x0081CD00 is the _M_fill_insert of the resize 0x0081D980 that the landed CExpressionSet::TransferBinaryIn 0x0081B510 runs on Expressions (Ego_r.pdb CVectorMap<EExpressionType, CExpressionSet::CEntry, ...>, stored as a CArray of std::pair<EExpressionType, CEntry> per FableWin.pdb ??4?$CArray@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@@@); the 40-byte stride is the retail one (Ego_r.pdb's CEntry is smaller).
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
enum EExpressionType { EXPRESSION_TYPE_0 = 0 };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CExpressionSet {
public:
    class CEntry {                                           // 36 bytes in TLC (the 40-byte pair stride)
    public:
        long Data[9];
    };
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>   // inline in every retail body
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template std::pair<EExpressionType, CExpressionSet::CEntry>* __fastcall __copy_backward_ptrs<std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*>(std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, std::pair<EExpressionType, CExpressionSet::CEntry>*, const __false_type&);
}