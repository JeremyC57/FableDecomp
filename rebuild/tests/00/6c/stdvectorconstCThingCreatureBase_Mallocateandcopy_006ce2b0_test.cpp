// Behaviour test for 0x006CE2B0 vector<T*>::_M_allocate_and_copy: n slots are allocated (none for n == 0) and
// [first, last) is copied to the front of the new block, which is returned.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CThingCreatureBase { int m_id; };
typedef const CThingCreatureBase* Elem;

static void* CopyTrivial(const void* first, const void* last, void* result)
{
    size_t n = (const char*)last - (const char*)first;
    if (n == 0) return result;
    return (char*)std::memmove(result, first, n) + n;
}

static Elem* AllocateAndCopy(unsigned int n, const Elem* first, const Elem* last)
{
    Elem* result = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
    CopyTrivial(first, last, result);
    return result;
}

int main()
{
    CThingCreatureBase a, b, c;
    Elem src[3] = { &a, &b, &c };
    Elem* p = AllocateAndCopy(8, src, src + 3);
    if (p == 0 || p[0] != &a || p[1] != &b || p[2] != &c) { std::printf("006ce2b0_TEST FAIL\n"); return 1; }
    std::free(p);
    Elem* q = AllocateAndCopy(0, src, src);
    if (q != 0) { std::printf("006ce2b0_TEST FAIL\n"); return 1; }
    std::printf("006ce2b0_TEST PASS\n");
    return 0;
}