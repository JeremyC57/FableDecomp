// Behaviour test for 0x005796D1 vector<CComponent*>::_M_allocate_and_copy: a fresh n-pointer buffer whose first
// (last - first) slots hold the source pointers; n == 0 gives null (no allocation).
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_allocs = 0;
static void* CopyTrivial(const void* first, const void* last, void* result)
{
    return last == first ? result
                         : (char*)std::memmove(result, first, (const char*)last - (const char*)first) +
                               ((const char*)last - (const char*)first);
}
static int** AllocateAndCopy(unsigned int n, int* const* first, int* const* last)
{
    int** result = n != 0 ? (++g_allocs, (int**)std::malloc(n * sizeof(int*))) : 0;
    CopyTrivial(first, last, result);
    return result;
}

int main()
{
    int a, b, c;
    int* src[3] = { &a, &b, &c };
    int** p = AllocateAndCopy(5, src, src + 3);
    bool ok = p != 0 && p != src && p[0] == &a && p[1] == &b && p[2] == &c && g_allocs == 1;
    std::free(p);
    ok = ok && AllocateAndCopy(0, src, src) == 0 && g_allocs == 1;
    std::printf(ok ? "005796d1_TEST PASS\n" : "005796d1_TEST FAIL\n");
    return ok ? 0 : 1;
}