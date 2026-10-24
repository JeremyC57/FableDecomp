// std::__copy(const CGameMapCell* first, const CGameMapCell* last, CGameMapCell* result, const random_access_iterator_tag&, int*) -- STLport counted forward copy of 7-byte game-map cells, returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7920, 81 bytes, `ret 0Ch`.
// Callers: 0x006B7B3C, 0x006B7E07, 0x006B8228 (CGameMap vector<CGameMapCell> erase / operator= / insert helpers).
// Byte twin: 0x006B79A0 (a separate COMDAT copy from another TU; graded and landed separately).
// Types (Ego_r.pdb): CGameMapCell = AtmosThemes uchar[3] +0, AtmosBlends uchar[2] +3, SoundTheme schar +5, Music uchar +6
//   (7 bytes, alignment 1, empty user default constructor as in the landed CGameMap ctor 0x006B7760).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` from a /Gr TU
//   (`__fastcall`: first in ecx, last in edx, result + the tag reference + the null distance pointer on the stack): the
//   signed count `last - first` once (`imul 92492493h ; add ; sar 2` = /7), then one 7-byte block assignment (dword +
//   word + byte) per element; the untouched result is returned for an empty range from its own tail.
// Label correction: the catalog `_Copy_impl<CGameMapCell*,CGameMapCell*>` is the Dinkumware spelling; retail STL is
//   STLport (the `ret 0Ch` + tag = __copy, GOTCHAS chunk 351).
struct CGameMapCell {                                   // 7 bytes
    unsigned char AtmosThemes[3];                       // +0
    unsigned char AtmosBlends[2];                       // +3
    signed char   SoundTheme;                           // +5
    unsigned char Music;                                // +6
    CGameMapCell() { }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template CGameMapCell* __fastcall __copy<const CGameMapCell*, CGameMapCell*, int>(const CGameMapCell*, const CGameMapCell*,
                                                                                  CGameMapCell*, const random_access_iterator_tag&, int*);
}