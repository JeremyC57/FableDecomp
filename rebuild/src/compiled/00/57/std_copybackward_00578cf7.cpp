// std::__copy_backward(ExperienceSpendInfo* first, ExperienceSpendInfo* last, ExperienceSpendInfo* result, const random_access_iterator_tag&, int*) -- STLport random-access backward copy of 12-byte spend records
// Leaf: ??$__copy_backward
// Retail: Fable.exe (TLC, VC7.1) @ 0x00578CF7, 49 bytes, __fastcall (first ecx, last edx) `ret 0Ch`. Called by the
//   12-byte vector's _M_fill_insert (0x00579B3C, this chunk) and 0x005790E7; the element is
//   CTCInventoryExperience::ExperienceSpendInfo (see 0x00578C50). The catalog "_Copy_backward<CBrainUpdateZone*,...>"
//   is propagated.
// Shape: STLport 4.x verbatim: `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last;
//   return __result;` -- signed count (`push 0Ch ; cdq ; pop ecx ; idiv`, size TU), `last` in ebx, `result`
//   decremented in its stack slot, the implicit 12-byte assignment as three `movsd`.
#pragma optimize("s", on)

class CTCInventoryExperience {
public:
    class ExperienceSpendInfo {
    public:
        long currentSpend;                          // +0x0
        long initialSpend;                          // +0x4
        long levelUpSpend;                          // +0x8
    };
};

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

template CTCInventoryExperience::ExperienceSpendInfo* __fastcall
__copy_backward(CTCInventoryExperience::ExperienceSpendInfo*, CTCInventoryExperience::ExperienceSpendInfo*,
                CTCInventoryExperience::ExperienceSpendInfo*, const random_access_iterator_tag&, int*);

}