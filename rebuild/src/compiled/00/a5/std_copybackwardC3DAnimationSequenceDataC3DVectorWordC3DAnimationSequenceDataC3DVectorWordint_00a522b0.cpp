// _global::CopyBackward_VectorWord_00a522b0(C3DVectorWord*, C3DVectorWord*, C3DVectorWord*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 6-byte C3DVectorWord elements
// Leaf: CopyBackward_VectorWord_00a522b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A522B0, length 71 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`). The count is
// (last - first) / 6 (`imul 2AAAAAABh`, no shift), each step moves both pointers back one element and copies a 6-byte
// POD (one dword + one word through esi/edi); the moved-back result is returned. Element identity: the manifest's
// `pair<float,CCharString>` label is propagated -- a 6-byte element cannot hold a float plus a CCharString; the body
// sits beside the C3DAnimationSequenceData::C3DVectorWord forward copies (0x00A52120/0x00A52260/0x00A744B0) and is
// their copy_backward twin.
struct C3DVectorWord {                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DVectorWord* __fastcall CopyBackward_VectorWord_00a522b0(C3DVectorWord* first, C3DVectorWord* last, C3DVectorWord* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}