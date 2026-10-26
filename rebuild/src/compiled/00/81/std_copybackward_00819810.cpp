// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (CParticleMorphs::CEntry*, CParticleMorphs::CEntry*, int) STLport random-access backward copy of trivially-assigned records: n = last - first block copies `*--result = *--last`, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAVCEntry@CParticleMorphs@@PAV12@H@std@@YIPAVCEntry@CParticleMorphs@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819810, 68 bytes (+ int3 padding), __fastcall (/Gr: first ecx, last edx; result,
//   the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (probable): std::__copy_backward<CParticleMorphs::CEntry*, CParticleMorphs::CEntry*, int>; evidence: unreferenced; it sits in CParticleMorphs's object code among the 0x24-stride helpers of vector<CParticleMorphs::CEntry> (Ego_r.pdb CParticleMorphs::CEntry, 36 bytes): the range erase 0x0081C0F0 (J102) under the resize 0x0081DB00 that CParticleMorphs::GetAllMorphsOfType 0x0081B160 calls on its output vector; FableWin.pdb holds vector<CParticleMorphs::CEntry>::resize / erase (the class's own CVectorMap holds 0x28-byte pair<EHeroMorphType, CEntry>).
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is a propagated Dinkumware spelling (family J101: five backward copies of 0x24- to
//   0x34-byte records; retail has no identical-code folding).
// Callees: none (the 0x24-byte assignment is one `rep movsd` of 9 dwords).
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim (J078's model over a trivially-assigned element); the
//   count is a division by 0x24 (multiply by 0x38E38E39).
enum EHeroMorphType { };                                  // Ego_r.pdb (enumerators not needed)
enum EMeshLocationPoint { };
class CDefString {                                         // 4 bytes (Ego_r.pdb)
public:
    long TablePos;                                         // +0x00
};
class CParticleMorphs {                                    // partial view: only the nested type is used here
public:
    class CEntry {                                         // 36 bytes (Ego_r.pdb; class key V from FableWin.pdb)
    public:
        EHeroMorphType MorphType;                          // +0x00
        bool Blending;                                     // +0x04
        EMeshLocationPoint Location;                       // +0x08
        CDefString LocationName;                           // +0x0C
        long LocationIndex;                                // +0x10
        long ParticleIndex;                                // +0x14
        long SecondParticleIndex;                          // +0x18
        float StartAtMorphStrength;                        // +0x1C
        float EndAtMorphStrength;                          // +0x20
    };
};
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
typedef CParticleMorphs::CEntry TElement_00819810;
template TElement_00819810* __copy_backward<TElement_00819810*, TElement_00819810*, int>(TElement_00819810*, TElement_00819810*, TElement_00819810*,
                                                                         const random_access_iterator_tag&, int*);
}