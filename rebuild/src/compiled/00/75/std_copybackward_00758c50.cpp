// std::copy_backward(first, last, result) -- (std::pair<EBodyLocation, CWoundMorphs::CEntry>) STLport backward copy of 44-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 11-dword `rep movsd`); returns result - n
// Leaf: ??$copy_backward@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@PAU12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00758C50, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result on the stack), `ret 4`.
// Identity: element std::pair<EBodyLocation, CWoundMorphs::CEntry> -- probable; evidence: its only caller is the landed, unreferenced __linear_insert 0x00758E00 (0x2C-byte records keyed by a signed long at +0) in the CWoundMorphs TU, whose only 0x2C-byte sorted record is the vector-map pair<EBodyLocation, CWoundMorphs::CEntry> (the insertion sort 0x00758FC0 and CWoundMorphs::SortMap 0x007597C0 sort it by its key); `ret 4`: three arguments, so it is copy_backward itself, not the __copy_backward_ptrs twin 0x007587A0 of the same pair.
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above), reached through STLport's copy_backward -> __copy_backward_aux -> __copy_backward_ptrs dispatch (the record is not trivially assignable to STLport's __type_traits), all inlined into this out-of-line copy_backward; `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
enum EBodyLocation { BODY_LOCATION_0 = 0 };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CWoundMorphs {
public:
    class CEntry {                                           // 40 bytes (Ego_r.pdb)
    public:
        EBodyLocation Location;
        long ReplacingTextureIndex, ReplacingBumpMapIndex, WoundTextureIndex, WoundBumpMapIndex;
        long ScarTextureIndex, ScarBumpMapIndex, FadeToScarDuration, TotalDuration;
        float PermanentScarStrength;
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

template <class _BidirectionalIter1, class _BidirectionalIter2>     // the record is not trivially assignable to STLport
inline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template std::pair<EBodyLocation, CWoundMorphs::CEntry>* __fastcall copy_backward<std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*>(std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*);
}