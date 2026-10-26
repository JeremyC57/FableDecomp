// behaviour test for 0x0045493E std::__uninitialized_copy<const long*, long*>: the POD run is copied byte for byte, the end of the copied run is
// returned, and an empty range returns result untouched
#include <cstdio>
#include <cstring>
struct TrueType { };
static long* UninitializedCopy(const long* first, const long* last, long* result, const TrueType&)   // the function under test
{
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return last == first ? result : (long*)((char*)std::memmove(result, first, n) + n);
}
int main()
{
    int fail = 0;
    long src[5], dst[7];
    std::memset(src, 0, sizeof(src)); std::memset(dst, 0, sizeof(dst));
    for (unsigned int i = 0; i < 5u; ++i) src[i] = (long)(i * 7 + 3) * 1000003L;
    long* end = UninitializedCopy(src, src + 5, dst + 1, TrueType());
    if (end != dst + 6) fail |= 1;
    if (std::memcmp(dst + 1, src, sizeof(src)) != 0) fail |= 2;
    if (dst[0] != (long)0 || dst[6] != (long)0) fail |= 4;
    if (UninitializedCopy(src + 2, src + 2, dst, TrueType()) != dst || dst[0] != (long)0) fail |= 8;
    if (fail) { std::printf("0045493e_TEST FAIL %d\n", fail); return 1; }
    std::printf("0045493e_TEST PASS\n");
    return 0;
}