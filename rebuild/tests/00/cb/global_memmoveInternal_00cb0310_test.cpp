// Behaviour test for 0x00CB0310 memmoveInternal (see src/00cb0310.cpp): overlap-safe move, returns dst.
#include <cstdio>
#include <cstring>
static void* MoveModel(void* dst, const void* src, unsigned int count)
{
    char* d = (char*)dst; const char* s = (const char*)src;
    if (d <= s || d >= s + count) { for (unsigned int i = 0; i < count; ++i) d[i] = s[i]; }
    else { while (count--) d[count] = s[count]; }
    return dst;
}
int main()
{
    char a[16] = "abcdefghij";
    if (MoveModel(a + 2, a, 7) != a + 2 || std::memcmp(a, "ababcdefgj", 10) != 0) { std::printf("00cb0310_TEST FAIL\n"); return 1; }
    char b[16] = "abcdefghij";
    MoveModel(b, b + 3, 6);
    if (std::memcmp(b, "defghighij", 10) != 0) { std::printf("00cb0310_TEST FAIL\n"); return 1; }
    std::printf("00cb0310_TEST PASS\n"); return 0;
}