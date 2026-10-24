// std::__copy(CHeroMarriageInfo* first, CHeroMarriageInfo* last, CHeroMarriageInfo* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of 24-byte marriage records
// Leaf: ??$__copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F29E, 51 bytes, __fastcall (first ecx, last edx) `ret 0Ch`. Called by the
//   CHeroMarriageInfo vector's assignment (0x0058034C) and 0x0057F929 (retail keeps the hero's marriages in a
//   vector: GFSerialiseVectorBinaryOut<CHeroMarriageInfo> 0x0057F109, the landed TransferBinaryOut 0x0057A3CD).
//   The catalog "_Copy_impl<CDegradableInfo*,...>" is propagated.
// Shape: STLport 4.x verbatim (as 0x00578C50): signed count by `push 18h ; cdq ; pop ecx ; idiv`, the implicit
//   24-byte assignment as `push 6 ; pop ecx ; rep movsd`, `result` advanced in its stack slot.
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

namespace std {

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

template CHeroMarriageInfo* __fastcall
__copy(CHeroMarriageInfo*, CHeroMarriageInfo*, CHeroMarriageInfo*, const random_access_iterator_tag&, int*);

}