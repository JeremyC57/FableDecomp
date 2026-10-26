// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, int) STLport random-access backward copy of trivially-assigned records: n = last - first block copies `*--result = *--last`, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@PAU12@H@std@@YIPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007582E0, 68 bytes (+ int3 padding), __fastcall (/Gr: first ecx, last edx; result,
//   the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (probable): std::__copy_backward<std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, int>; evidence: unreferenced; it sits in CWoundMorphs's object code among the 0x2C-stride helpers of CWoundMorphs::Morphs, Ego_r.pdb CVectorMap<EBodyLocation, CWoundMorphs::CEntry, ...> = vector<pair<EBodyLocation, CWoundMorphs::CEntry> > (0x2C): its range erase 0x00758F50 (J102) under the resize 0x007595F0 that CWoundMorphs::TransferBinaryIn 0x00757990 calls; FableWin.pdb holds that vector's erase and resize.
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is a propagated Dinkumware spelling (family J101: five backward copies of 0x24- to
//   0x34-byte records; retail has no identical-code folding).
// Callees: none (the 0x2C-byte assignment is one `rep movsd` of 11 dwords).
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim (J078's model over a trivially-assigned element); the
//   count is a division by 0x2C (multiply by 0x2E8BA2E9).
enum EBodyLocation { };                                   // Ego_r.pdb (enumerators not needed)
class CWoundMorphs {                                       // partial view: only the nested type is used here
public:
    class CEntry {                                         // 40 bytes (Ego_r.pdb; class key V from FableWin.pdb)
    public:
        EBodyLocation Location;                            // +0x00
        long ReplacingTextureIndex;                        // +0x04
        long ReplacingBumpMapIndex;                        // +0x08
        long WoundTextureIndex;                            // +0x0C
        long WoundBumpMapIndex;                            // +0x10
        long ScarTextureIndex;                             // +0x14
        long ScarBumpMapIndex;                             // +0x18
        long FadeToScarDuration;                           // +0x1C
        long TotalDuration;                                // +0x20
        float PermanentScarStrength;                       // +0x24
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
};
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
typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> TElement_007582e0;
template TElement_007582e0* __copy_backward<TElement_007582e0*, TElement_007582e0*, int>(TElement_007582e0*, TElement_007582e0*, TElement_007582e0*,
                                                                         const random_access_iterator_tag&, int*);
}