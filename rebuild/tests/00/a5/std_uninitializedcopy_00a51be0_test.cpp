// behaviour test for 0x00A51BE0 std::__uninitialized_copy [CQuaternion]: every element of [first, last) is copy-constructed in order into the storage at result, the end of the constructed run is returned, an empty range constructs nothing
#include <cstdio>
#include <new>
struct Elem { float x, y, z, w; };                          // 16 bytes, implicit copy constructor
static Elem* UninitializedCopy(const Elem* first, const Elem* last, Elem* result)   // the function under test
{
    Elem* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Elem(*first);
    return cur;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != (unsigned int)16) fail |= 1;
    Elem src[3];
    for (int i = 0; i < 3; ++i) { src[i].x = 1.0f + i; src[i].y = 2.0f + i; src[i].z = 3.0f + i; src[i].w = 4.0f + i; }
    Elem dst[4];
    for (int i = 0; i < 4; ++i) { dst[i].x = dst[i].y = dst[i].z = dst[i].w = -1.0f; }
    Elem* end = UninitializedCopy(src, src + 3, dst);
    if (end != dst + 3) fail |= 2;
    for (int i = 0; i < 3; ++i)
        if (dst[i].x != src[i].x || dst[i].y != src[i].y || dst[i].z != src[i].z || dst[i].w != src[i].w) fail |= 4;
    if (dst[3].x != -1.0f) fail |= 8;
    if (UninitializedCopy(src, src, dst + 3) != dst + 3 || dst[3].w != -1.0f) fail |= 16;
    if (fail) { std::printf("00a51be0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a51be0_TEST PASS\n");
    return 0;
}