// Behaviour test for 0x006715A0 std::vector<ENavigatorType>::_M_allocate_and_copy (model copy)
#include <cstdio>
#include <cstdlib>
enum ENav { N0, N1, N2, N3 };
static ENav* AllocateAndCopy(unsigned int n, const ENav* first, const ENav* last)
{
    ENav* result = n != 0 ? (ENav*)std::malloc(n * sizeof(ENav)) : 0;
    ENav* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) *cur = *first;
    return result;
}
int main()
{
    ENav src[3] = { N2, N0, N3 };
    ENav* p = AllocateAndCopy(5, src, src + 3);
    bool ok = p != 0 && p[0] == N2 && p[1] == N0 && p[2] == N3;
    std::free(p);
    ok = ok && AllocateAndCopy(0, src, src) == 0;
    std::printf(ok ? "006715a0_TEST PASS\n" : "006715a0_TEST FAIL\n");
    return ok ? 0 : 1;
}