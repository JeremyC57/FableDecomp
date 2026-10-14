#include <cstdio>
#include <string.h>
struct CByteVector_00567ac0 {
    unsigned char* first; unsigned char* last; unsigned char* end_of_storage;
    static unsigned char* CopyDown(unsigned char* src, unsigned char* srcEnd, unsigned char* dest) { if (src == srcEnd) return dest; unsigned long count = srcEnd - src; return (unsigned char*)memmove(dest, src, count) + count; }
    void Erase(unsigned char* eraseFirst, unsigned char* eraseLast) { last = CopyDown(eraseLast, last, eraseFirst); }
    void Clear() { Erase(first, last); }
};
int main()
{
    unsigned char buf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; CByteVector_00567ac0 v; v.first = buf; v.last = buf + 8; v.end_of_storage = buf + 8;
    v.Erase(buf + 1, buf + 3);                                  // 1 4 5 6 7 8
    if (v.last != buf + 6 || buf[1] != 4 || buf[5] != 8) { std::printf("00567ac0_TEST FAIL\n"); return 1; }
    v.Clear();
    if (v.last != v.first || v.first != buf || buf[0] != 1) { std::printf("00567ac0_TEST FAIL\n"); return 1; }
    v.Clear();
    if (v.last != buf) { std::printf("00567ac0_TEST FAIL\n"); return 1; }
    std::printf("00567ac0_TEST PASS\n"); return 0;
}