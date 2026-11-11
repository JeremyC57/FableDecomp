// Behaviour test for 0x0082C420 UninitCopy_Dword_0082c420: placement copy of 4-byte records, returns the advanced result.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SDword { unsigned long m_value; };

static SDword* UninitCopy(const SDword* first, const SDword* last, SDword* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SDword(*first);
    return result;
}

int main()
{
    SDword src[4];
    SDword dst[5];
    for (int i = 0; i < 4; ++i) { src[i].m_value = 0x1000u + i * 3u; }
    for (int j = 0; j < 5; ++j) { dst[j].m_value = 0xDEADu; }
    __false_type tag;
    SDword* r = UninitCopy(src, src + 4, dst, tag);
    bool ok = r == dst + 4 && dst[4].m_value == 0xDEADu;
    for (int k = 0; k < 4; ++k)
        if (dst[k].m_value != 0x1000u + k * 3u) ok = false;
    if (UninitCopy(src, src, dst + 2, tag) != dst + 2) ok = false;   // empty range returns result untouched
    if (!ok) { std::printf("0082c420_TEST FAIL\n"); return 1; }
    std::printf("0082c420_TEST PASS\n");
    return 0;
}