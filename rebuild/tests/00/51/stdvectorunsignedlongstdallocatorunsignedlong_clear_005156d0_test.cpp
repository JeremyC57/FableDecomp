#include <cstdio>
#include <cstring>
static inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result : ((char*)std::memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
struct V { char* first; char* last; char* e; void clear() { char* n = (char*)CopyTrivial(last, last, first); last = n; } };
int main()
{
    char buf[8] = "abcdefg"; V v; v.first = buf; v.last = buf + 5; v.e = buf + 8; v.clear();
    if (v.last != buf || v.first != buf || buf[0] != 'a') { std::printf("005156d0_TEST FAIL\n"); return 1; }
    v.clear(); if (v.last != buf) { std::printf("005156d0_TEST FAIL\n"); return 1; }
    std::printf("005156d0_TEST PASS\n");
    return 0;
}