// _global::CopyBackward_Element7_006b7a00(SElement7*, SElement7*, SElement7*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 7-byte POD records
// Leaf: CopyBackward_Element7_006b7a00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7A00, length 84 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`). The count is
// (last - first) / 7 (`imul 92492493h ; add ; sar 2`): the element has a 7-byte stride, so it is a byte-aligned record,
// and its implicit operator= copies dword + word + byte. The moved-back result is returned. The manifest's
// `pair<float,CCharString>` label is propagated (that pair is 8+ bytes); the callers (0x006B7BFC, 0x006B80FB, vector
// insert helpers of the same TU) do not name the element type, so it stays a descriptive 7-byte record.
struct SElement7 {                     // 7-byte record (stride 7, no alignment padding)
    unsigned char _body[7];
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SElement7* __fastcall CopyBackward_Element7_006b7a00(SElement7* first, SElement7* last, SElement7* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}