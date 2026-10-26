// behaviour test for 0x0045BCC1 std::__uninitialized_copy<const unsigned long*, unsigned long*>: the POD run is copied byte for byte, the end of the copied run is
// returned, and an empty range returns result untouched
#include <cstdio>
#include <cstring>
struct TrueType { };
static unsigned long* UninitializedCopy(const unsigned long* first, const unsigned long* last, unsigned long* result, const TrueType&)   // the function under test
{
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return last == first ? result : (unsigned long*)((char*)std::memmove(result, first, n) + n);
}
int main()
{
    int fail = 0;
    unsigned long src[5], dst[7];
    std::memset(src, 0, sizeof(src)); std::memset(dst, 0, sizeof(dst));
    for (unsigned int i = 0; i < 5u; ++i) src[i] = (unsigned long)0x80000000UL + i * 9u;
    unsigned long* end = UninitializedCopy(src, src + 5, dst + 1, TrueType());
    if (end != dst + 6) fail |= 1;
    if (std::memcmp(dst + 1, src, sizeof(src)) != 0) fail |= 2;
    if (dst[0] != (unsigned long)0 || dst[6] != (unsigned long)0) fail |= 4;
    if (UninitializedCopy(src + 2, src + 2, dst, TrueType()) != dst || dst[0] != (unsigned long)0) fail |= 8;
    if (fail) { std::printf("0045bcc1_TEST FAIL %d\n", fail); return 1; }
    std::printf("0045bcc1_TEST PASS\n");
    return 0;
}