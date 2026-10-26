// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CEngineOcclusionManager::COccluderFlareEntry elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@H@std@@YIPAUCOccluderFlareEntry@CEngineOcclusionManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8E980, 73 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (count by `imul 92492493h ; sar edx,4`).
// Identity (proven): __copy_backward<CEngineOcclusionManager::COccluderFlareEntry*, CEngineOcclusionManager::COccluderFlareEntry*, int>; evidence: its call site 0x00B8F59C is in the _M_fill_insert 0x00B8F4E0 of resize(n, x) 0x00B8F780, which the landed CEngineOcclusionManager::FrameStart 0x00B8DBE0 calls on this+0x24024: FlareTests, CArray<CEngineOcclusionManager::COccluderFlareEntry> (a 28-byte struct; FableWin.pdb ??0?$CArray@UCOccluderFlareEntry@CEngineOcclusionManager@@).
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is propagated across family H040 (six bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 28-byte element (the assignment is `rep movsd` with ecx = 7).
class CEngineOcclusionManager {
public:
    struct COccluderFlareEntry {                        // Ego_r.pdb, 28 bytes: Position, Radius, CircleSegments, MaxPixelCount, OccluderHandle
        long Word[7];
    };
};
namespace std {
struct random_access_iterator_tag {};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}
typedef CEngineOcclusionManager::COccluderFlareEntry TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);