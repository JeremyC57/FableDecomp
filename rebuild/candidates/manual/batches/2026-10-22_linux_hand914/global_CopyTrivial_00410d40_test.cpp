// Behaviour test for 00410d40 CopyTrivial (STLport __copy_trivial)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
char* __fastcall CopyTrivial(const char* first, const char* last, char* result)
{
    return (last == first) ? result
                           : (char*)std::memmove(result, first, last - first) + (last - first);
}
int main()
{
    const char src[] = "abcdef";
    char dst[8]; std::memset(dst, 'x', sizeof dst);
    char* end = CopyTrivial(src, src + 4, dst);
    if (end != dst + 4 || std::memcmp(dst, "abcdxx", 6) != 0) { std::printf("00410d40_TEST FAIL\n"); return 1; }
    if (CopyTrivial(src, src, dst) != dst) { std::printf("00410d40_TEST FAIL\n"); return 1; }
    char buf[8] = "123456";
    end = CopyTrivial(buf, buf + 4, buf + 2);   // overlapping (memmove)
    if (end != buf + 6 || std::memcmp(buf, "121234", 6) != 0) { std::printf("00410d40_TEST FAIL\n"); return 1; }
    std::printf("00410d40_TEST PASS\n");
    return 0;
}
