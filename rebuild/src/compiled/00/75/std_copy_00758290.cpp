// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (std::pair<EBodyLocation, CWoundMorphs::CEntry>)
// Leaf: ??$__copy@PAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@PAU12@H@std@@YIPAU?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758290, 68 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (probable): no call, jump or data reference reaches it (unreferenced COMDAT); its 44-byte stride is pair<EBodyLocation, CWoundMorphs::CEntry> (Ego_r.pdb CWoundMorphs::CEntry, 40 bytes), whose fill 0x00758220 (bound ??$fill@U?$pair@W4EBodyLocation@@VCEntry@CWoundMorphs@@@std@@...) is its neighbour; the source iterator's constness is not visible.
// Callees: none.
// Shape (family L058): n = (last - first) / 44 by the magic divide; each trivial assignment is `mov ecx,11 ; rep movsd`
//   with the two cursors stepped beside it.
enum EBodyLocation { };
class CWoundMorphs {
public:
    class CEntry {                                         // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                           // +0x00
        long ReplacingTextureIndex;                       // +0x04
        long ReplacingBumpMapIndex;                       // +0x08
        long WoundTextureIndex;                           // +0x0C
        long WoundBumpMapIndex;                           // +0x10
        long ScarTextureIndex;                            // +0x14
        long ScarBumpMapIndex;                            // +0x18
        long FadeToScarDuration;                          // +0x1C
        long TotalDuration;                               // +0x20
        float PermanentScarStrength;                      // +0x24
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template std::pair<EBodyLocation, CWoundMorphs::CEntry>* __fastcall std::__copy(std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, const std::random_access_iterator_tag&, int*);