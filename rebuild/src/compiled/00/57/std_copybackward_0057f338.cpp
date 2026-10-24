// std::__copy_backward(CHeroMarriageInfo* first, CHeroMarriageInfo* last, CHeroMarriageInfo* result, const random_access_iterator_tag&, int*) -- STLport random-access backward copy of 24-byte marriage records
// Leaf: ??$__copy_backward
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F338, 51 bytes, __fastcall (first ecx, last edx) `ret 0Ch`. Called by the
//   CHeroMarriageInfo vector's range insert (0x00580928) and 0x0057F97D (see 0x0057F29E for the retail vector).
//   The catalog "_Copy_backward<CHeroMarriageInfo*,...>" names the element correctly; the spelling is STLport's.
// Shape: STLport 4.x verbatim (as 0x00578CF7): signed count by `push 18h ; cdq ; pop ecx ; idiv`, the implicit
//   24-byte assignment as `push 6 ; pop ecx ; rep movsd`.
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

namespace std {

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CHeroMarriageInfo* __fastcall
__copy_backward(CHeroMarriageInfo*, CHeroMarriageInfo*, CHeroMarriageInfo*, const random_access_iterator_tag&, int*);

}