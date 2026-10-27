// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (CUnprovenRecord_008193f0)
// Leaf: ??$__copy@PAVCUnprovenRecord_008193f0@@PAV1@H@std@@YIPAVCUnprovenRecord_008193f0@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008193f0, 68 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT); its own bytes copy 40-byte records as 10-dword blocks; Ego_r.pdb has several trivially assigned 40-byte records (CWoundMorphs::CEntry, pair<EHeroMorphType, CParticleMorphs::CEntry>, ...) and nothing here picks one.
// Callees: none.
// Shape (family L058): n = (last - first) / 40 by the magic divide; each trivial assignment is `mov ecx,10 ; rep movsd`
//   with the two cursors stepped beside it.
class CUnprovenRecord_008193f0 {                          // descriptive: a trivially assigned 40-byte record
public:
    unsigned long Words[10];                              // +0x00
};
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
template CUnprovenRecord_008193f0* __fastcall std::__copy(CUnprovenRecord_008193f0*, CUnprovenRecord_008193f0*, CUnprovenRecord_008193f0*, const std::random_access_iterator_tag&, int*);