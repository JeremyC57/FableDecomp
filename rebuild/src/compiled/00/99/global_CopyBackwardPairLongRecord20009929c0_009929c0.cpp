// _global::CopyBackward_PairLongRecord20_009929c0(SPairLongRecord20*, SPairLongRecord20*, SPairLongRecord20*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 0x24-byte (key, 0x20-byte record) pairs
// Leaf: CopyBackward_PairLongRecord20_009929c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009929C0, length 76 bytes (+ int3 padding).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T), both pointers step back one element and the moved-back result is returned.
// Element: the 0x24-byte pair of the forward copy 0x00992A10 (same TU): a long key + a 0x20-byte record assigned
// through its out-of-line operator= @ 0x00992630. The manifest's `pair<float,CCharString>` label is propagated.
struct SRecord20 {                           // 0x20 bytes
    unsigned char _body[0x20];
    SRecord20& operator=(const SRecord20& other);         // 0x00992630
};

struct SPairLongRecord20 {                   // 0x24 bytes
    long      first;                         // +0x00
    SRecord20 second;                        // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SPairLongRecord20* __fastcall CopyBackward_PairLongRecord20_009929c0(SPairLongRecord20* first, SPairLongRecord20* last, SPairLongRecord20* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}