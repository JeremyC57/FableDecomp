// behaviour test for 0x00477C18 vector<CEngineLocalDetailObjectDef>::_M_allocate_and_copy: n elements are allocated, [first, last) is copy-constructed into the
// block front to back (one copy constructor per element), and n == 0 allocates nothing
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct Elem {                                               // stands in for the 0x88-byte CEngineLocalDetailObjectDef
    unsigned char body[0x88];
    Elem() { for (unsigned int i = 0; i < (unsigned int)sizeof(body); ++i) body[i] = 0; }
    Elem(const Elem& o) { ++g_copies; for (unsigned int i = 0; i < (unsigned int)sizeof(body); ++i) body[i] = o.body[i]; }
};
static Elem* AllocateAndCopy(unsigned int n, const Elem* first, const Elem* last)       // the function under test
{
    Elem* result = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
    Elem* cur = result;
    for (; first != last; ++first, ++cur) new (cur) Elem(*first);
    return result;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 0x88u) fail |= 1;
    Elem src[3];
    for (unsigned int k = 0; k < 3u; ++k) for (unsigned int i = 0; i < (unsigned int)sizeof(src[k].body); ++i) src[k].body[i] = (unsigned char)(k * 31 + i);
    Elem* p = AllocateAndCopy(4, src, src + 3);
    if (p == 0 || g_copies != 3) fail |= 2;
    for (unsigned int k = 0; p && k < 3u; ++k) for (unsigned int i = 0; i < (unsigned int)sizeof(src[k].body); ++i) if (p[k].body[i] != src[k].body[i]) fail |= 4;
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0 || g_copies != 3) fail |= 8;
    if (fail) { std::printf("00477c18_TEST FAIL %d\n", fail); return 1; }
    std::printf("00477c18_TEST PASS\n");
    return 0;
}