// Behaviour test for 0x00660213 vector<long>::_M_allocate_and_copy (model copy)
#include <cstdio>
#include <cstdlib>
#include <cstring>
void* copy_trivial(const void* first, const void* last, void* result)
{
    return first == last ? result : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
long* allocate_and_copy(unsigned int n, long* first, long* last)
{
    long* result = n != 0 ? (long*)malloc(n * sizeof(long)) : 0;
    copy_trivial(first, last, result);
    return result;
}
int main()
{
    long src[3] = { 7, -8, 9 };
    long* p = allocate_and_copy(5, src, src + 3);
    if (p == 0 || p[0] != 7 || p[1] != -8 || p[2] != 9) { std::printf("00660213_TEST FAIL\n"); return 1; }
    if (allocate_and_copy(0, src, src) != 0) { std::printf("00660213_TEST FAIL\n"); return 1; }
    free(p);
    std::printf("00660213_TEST PASS\n");
    return 0;
}