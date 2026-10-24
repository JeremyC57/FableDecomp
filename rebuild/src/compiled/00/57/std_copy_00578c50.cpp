// std::__copy(ExperienceSpendInfo* first, ExperienceSpendInfo* last, ExperienceSpendInfo* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of 12-byte spend records
// Leaf: ??$__copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00578C50, 45 bytes, __fastcall (first ecx, last edx) `ret 0Ch`. Called by the
//   12-byte vector's erase (0x005798A3, the tail shift `copy(last, finish, first)`) and 0x00579044. The element is
//   CTCInventoryExperience::ExperienceSpendInfo {currentSpend, initialSpend, levelUpSpend} (the
//   ExperienceSpendValues* vectors; landed 0x005724CD); a class, so STLport's type traits send `copy` through
//   __copy_ptrs(__false_type) to this assignment loop. The catalog "_Move<CMusicSetEntry*,...>" is propagated.
// Shape: STLport 4.x verbatim: `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first;
//   ++__first; ++__result; } return __result;` -- the signed element count (`push 0Ch ; cdq ; pop esi ; idiv`,
//   size TU), the implicit 12-byte assignment as three `movsd`, `result` advanced in its stack slot.
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

template CTCInventoryExperience::ExperienceSpendInfo* __fastcall
__copy(CTCInventoryExperience::ExperienceSpendInfo*, CTCInventoryExperience::ExperienceSpendInfo*,
       CTCInventoryExperience::ExperienceSpendInfo*, const random_access_iterator_tag&, int*);

}