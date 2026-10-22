// Behaviour test for 00410d60 CopyTrivialBackward (STLport __copy_trivial_backward)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
char* __fastcall CopyTrivialBackward(const char* first, const char* last, char* result)
{
    const int n = last - first;
    return (n > 0) ? (char*)std::memmove(result - n, first, n) : result;
}
int main()
{
    char buf[8] = "123456";
    char* r = CopyTrivialBackward(buf, buf + 3, buf + 5);   // copy "123" to end at buf+5
    if (r != buf + 2 || std::memcmp(buf, "121236", 6) != 0) { std::printf("00410d60_TEST FAIL\n"); return 1; }
    if (CopyTrivialBackward(buf, buf, buf + 3) != buf + 3) { std::printf("00410d60_TEST FAIL\n"); return 1; }
    std::printf("00410d60_TEST PASS\n");
    return 0;
}