// _global::Copy_VectorWord_00a52120(C3DVectorWord*, C3DVectorWord*, C3DVectorWord*, const random_access_iterator_tag&, int*) -- SGI __copy over 6-byte C3DAnimationSequenceData::C3DVectorWord elements
// Leaf: Copy_VectorWord_00a52120
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52120, length 70 bytes (+ int3 padding).
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the manifest's
// Dinkumware `_Copy_impl` label is propagated; three byte-identical COMDAT copies sit at 0x00A52120, 0x00A52260 and
// 0x00A744B0). /Gr TU: __fastcall with first in ecx, last in edx, result + the tag reference + the distance pointer on
// the stack (`ret 0Ch`). The count is (last - first) / 6 (`imul 2AAAAAABh` with no shift); each element is the
// implicit operator= of a 6-byte POD of three shorts, which VC7.1 copies as one dword + one word through esi/edi.
// The result pointer advanced past the copied range is returned (the untouched result for an empty range).
struct C3DVectorWord {                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes (quantised x/y/z)
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DVectorWord* __fastcall Copy_VectorWord_00a52120(C3DVectorWord* first, C3DVectorWord* last, C3DVectorWord* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}