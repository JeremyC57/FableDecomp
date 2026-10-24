// _global::Copy_PairLongRecord20_00992a10(SPairLongRecord20*, SPairLongRecord20*, SPairLongRecord20*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x24-byte (key, 0x20-byte record) pairs
// Leaf: Copy_PairLongRecord20_00992a10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00992A10, length 75 bytes.
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element: 0x24 bytes = a long key + a 0x20-byte record assigned through its out-of-line operator= @ 0x00992630 (it
// starts with a counted-pointer release/adopt; catalogued as CGameCameraManager::UpdateCleanupFlags -- propagated). The
// implicit pair operator= loads the key, forms both record addresses, stores the key and calls the record's operator=.
// The manifest's `CTCSkeletalMorph::CSkeletalMorph` label is propagated (that type is 12 bytes); callers 0x00992C6C /
// 0x00993049 are vector helpers of the same TU, so the record stays descriptive.
struct SRecord20 {                           // 0x20 bytes
    unsigned char _body[0x20];
    SRecord20& operator=(const SRecord20& other);         // 0x00992630
};

struct SPairLongRecord20 {                   // 0x24 bytes
    long      first;                         // +0x00
    SRecord20 second;                        // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SPairLongRecord20* __fastcall Copy_PairLongRecord20_00992a10(SPairLongRecord20* first, SPairLongRecord20* last, SPairLongRecord20* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}