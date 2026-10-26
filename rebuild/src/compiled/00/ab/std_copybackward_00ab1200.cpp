// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*, int) STLport random-access backward copy of trivially-assigned records: n = last - first block copies `*--result = *--last`, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAVCSequenceInfo@N3DAnimationBuilder@@PAV12@H@std@@YIPAVCSequenceInfo@N3DAnimationBuilder@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1200, 68 bytes (+ int3 padding), __fastcall (/Gr: first ecx, last edx; result,
//   the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (probable): std::__copy_backward<N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*, int>; evidence: unreferenced; it sits in the C3DAnimationBuilder object code among the 0x34-stride helpers of vector<N3DAnimationBuilder::CSequenceInfo> (Ego_r.pdb, 52 bytes): the range erase 0x00AB2C70 (J102) under the resize 0x00AB3290 that C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520 calls; FableWin.pdb holds vector<N3DAnimationBuilder::CSequenceInfo>::resize.
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is a propagated Dinkumware spelling (family J101: five backward copies of 0x24- to
//   0x34-byte records; retail has no identical-code folding).
// Callees: none (the 0x34-byte assignment is one `rep movsd` of 13 dwords).
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim (J078's model over a trivially-assigned element); the
//   count is a division by 0x34 (multiply by 0x4EC4EC4F).
class CMatrix3x4 {                                         // 48 bytes (Ego_r.pdb: a union of rows / elements; opaque here)
public:
    unsigned char _body[0x30];
};
namespace N3DAnimationBuilder {
class CSequenceInfo {                                      // 52 bytes (Ego_r.pdb; class key V from FableWin.pdb)
public:
    long OriginalIndex;                                    // +0x00
    CMatrix3x4 GeneratedTransform;                         // +0x04
};
}
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef N3DAnimationBuilder::CSequenceInfo TElement_00ab1200;
template TElement_00ab1200* __copy_backward<TElement_00ab1200*, TElement_00ab1200*, int>(TElement_00ab1200*, TElement_00ab1200*, TElement_00ab1200*,
                                                                         const random_access_iterator_tag&, int*);
}