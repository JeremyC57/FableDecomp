// std::__copy(pair<const CRegion*, C3DVector>* first, pair<...>* last, pair<...>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of 16-byte house-position records
// Leaf: ??$__copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F188, 41 bytes, __fastcall (first ecx, last edx) `ret 0Ch`. Called by
//   vector::erase(iterator) @ 0x0057FD32 (from CTCHeroStats::RemoveHousePosition 0x0057ADF7) and 0x0057F62D. The
//   element is CTCHeroStats::HousePositions' value, pair<const CRegion*, C3DVector> (Ego_r.pdb; FableWin.pdb
//   `?RemoveHousePosition@CTCHeroStats@@QAEXABVCRegion@@ABVC3DVector@@@Z`). The catalog "_Move<CMusicEntry*,...>"
//   is propagated.
// Shape: STLport 4.x verbatim (as 0x00578C50): the 16-byte stride makes the count a plain `sar edx,4`; the pair's
//   implicit assignment is four `movsd`; `result` advanced in its stack slot.
#pragma optimize("s", on)

class CRegion;
class C3DVector {
public:
    float X;                                        // +0x0
    float Y;                                        // +0x4
    float Z;                                        // +0x8
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x0
    _T2 second;                                     // +0x4
};


struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template pair<const CRegion*, C3DVector>* __fastcall
__copy(pair<const CRegion*, C3DVector>*, pair<const CRegion*, C3DVector>*, pair<const CRegion*, C3DVector>*,
       const random_access_iterator_tag&, int*);

}