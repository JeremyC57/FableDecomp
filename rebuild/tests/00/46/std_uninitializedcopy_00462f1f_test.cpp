// behaviour test for 0x00462F1F std::__uninitialized_copy<const bool*, bool*>: the POD run is copied byte for byte, the end of the copied run is
// returned, and an empty range returns result untouched
#include <cstdio>
#include <cstring>
struct TrueType { };
static bool* UninitializedCopy(const bool* first, const bool* last, bool* result, const TrueType&)   // the function under test
{
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return last == first ? result : (bool*)((char*)std::memmove(result, first, n) + n);
}
int main()
{
    int fail = 0;
    bool src[5], dst[7];
    std::memset(src, 0, sizeof(src)); std::memset(dst, 0, sizeof(dst));
    for (unsigned int i = 0; i < 5u; ++i) src[i] = (bool)(i & 1) != 0 || i == 4;
    bool* end = UninitializedCopy(src, src + 5, dst + 1, TrueType());
    if (end != dst + 6) fail |= 1;
    if (std::memcmp(dst + 1, src, sizeof(src)) != 0) fail |= 2;
    if (dst[0] != (bool)0 || dst[6] != (bool)0) fail |= 4;
    if (UninitializedCopy(src + 2, src + 2, dst, TrueType()) != dst || dst[0] != (bool)0) fail |= 8;
    if (fail) { std::printf("00462f1f_TEST FAIL %d\n", fail); return 1; }
    std::printf("00462f1f_TEST PASS\n");
    return 0;
}