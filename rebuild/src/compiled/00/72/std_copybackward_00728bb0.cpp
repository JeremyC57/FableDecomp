// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAVCMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@PAV123@H@std@@YIPAVCMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@PAV123@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728BB0, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts (16 bytes); evidence: its callers (0x00728E69 in 0x00728DF0, the fill-insert under resize 0x00728F20, and 0x00728BFC) belong to the vector that CTCRandomAppearanceMorph::UpdateMorphSet 0x007280C0 resizes through `[this+0x10]` -- the CMeshMorphSet behind Ego_r.pdb's CTCRandomAppearanceMorph::PMeshMorphSet (+0x10), whose MeshParts (+0) is CArray<CMeshMorphSetParts>; CMeshMorphSetParts is {unsigned long MeshIndex; CEngineTextureMorphSet TextureMorphs}, and the default value the resize copies is destroyed as a vector at +4
// Label: the manifest's "_Copy_backward<std::pair<float,CCharString>*,std::pair<float,class_CCharString>*>" is propagated (family H070: six backward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar eax,3`) and 16-byte ones (`sar eax,4`).
// Callee: vector<CEngineTextureMorph>::operator= 0x00728830 (20-byte elements) on TextureMorphs.Morphs.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector& operator=(const vector& __x);              // out of line (0x00728830)
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
class CEngineTextureMorph;                           // 20 bytes in retail
class CEngineTextureMorphSet {                       // 12 bytes in retail
public:
    CArray<CEngineTextureMorph> Morphs;
};
class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                   // 16 bytes in retail
        public:
            unsigned long          MeshIndex;        // +0x00
            CEngineTextureMorphSet TextureMorphs;    // +0x04
        };
    };
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts* __fastcall __copy_backward<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, int>(CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std