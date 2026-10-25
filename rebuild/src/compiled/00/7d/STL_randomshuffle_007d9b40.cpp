// std::random_shuffle(KnickKnack* first, KnickKnack* last) -- STLport random shuffle of the spot-the-addition knick-knack records: swap each element with a rand()-chosen element at or before it
// Leaf: ??$random_shuffle@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D9B40, 159 bytes (+1 int3), /Gr __fastcall (first in ecx, last in edx),
//   plain `ret`.
// Identity: the only caller is CTCTavernGameSpotTheAddition::FrameUpdate (0x007D8E20, call at 0x007D92EC) over its
//   KnickKnacks vector (+0x110/+0x114); Ego_r.pdb names the donor instance random_shuffle<..._Vector_iterator<
//   KnickKnack>...> (YIX: __fastcall), and KnickKnack is 16 bytes {C3DVector Pos, CThingObject* Thing}. The catalog
//   label "std_random_shuffle_16byte" is descriptive.
// Callees: rand @ 0x00BFEB16.
// Shape: STLport `if (__first == __last) return; for (__i = __first + 1; __i != __last; ++__i) iter_swap(__i,
//   __first + __random_number((__i - __first) + 1));` with __random_number = `rand() % __n` and the value-type swap
//   (`_Tp __tmp = *__a; *__a = *__b; *__b = __tmp;`) inline; C3DVector is pack(1) with the engine's memberwise copy constructor (the
//   temporary's position words go to the frame, its thing pointer stays in eax).
extern "C" int __cdecl rand();                          // 0x00BFEB16

class CThingObject;
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
struct KnickKnack {                                     // 16 bytes
    C3DVector     Pos;                                  // +0x00
    CThingObject* Thing;                                // +0x0c
};

namespace std {
template <class _Distance> inline _Distance __random_number(_Distance __n) { return rand() % __n; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter>
void __fastcall random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        iter_swap(__i, __first + __random_number((__i - __first) + 1));
}

template void __fastcall random_shuffle<KnickKnack*>(KnickKnack*, KnickKnack*);
}