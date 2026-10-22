// Behaviour test for 00410cb0 CopyTrivialTagged (STLport __copy_trivial, __true_type tag)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
struct __true_type {};
char* __fastcall CopyTrivialTagged(const char* first, const char* last, char* result, __true_type)
{
    return (last == first) ? result
                           : (char*)std::memmove(result, first, last - first) + (last - first);
}
int main()
{
    const char src[] = "abcdef";
    char dst[8]; std::memset(dst, 'x', sizeof dst);
    char* end = CopyTrivialTagged(src, src + 4, dst, __true_type());
    if (end != dst + 4 || std::memcmp(dst, "abcdxx", 6) != 0) { std::printf("00410cb0_TEST FAIL\n"); return 1; }
    if (CopyTrivialTagged(src, src, dst, __true_type()) != dst) { std::printf("00410cb0_TEST FAIL\n"); return 1; }
    char buf[8] = "123456";
    end = CopyTrivialTagged(buf, buf + 4, buf + 2, __true_type());   // overlapping (memmove)
    if (end != buf + 6 || std::memcmp(buf, "121234", 6) != 0) { std::printf("00410cb0_TEST FAIL\n"); return 1; }
    std::printf("00410cb0_TEST PASS\n");
    return 0;
}