// Behaviour test for 0x00C29340 UninitCopy_Dword_00c29340: Dinkumware-style _Uninit_copy through an allocator's construct.
#include <cstdio>
#include <new>

struct STag { };
struct SDword { unsigned long m_value; };
static int g_constructs = 0;
struct SAllocator {
    void construct(SDword* p, const SDword& v) { new (p) SDword(v); ++g_constructs; }
};

static SDword* UninitCopy(const SDword* first, const SDword* last, SDword* dest, SAllocator& al, STag)
{
    for (; first != last; ++dest, ++first)
        al.construct(dest, *first);
    return dest;
}

int main()
{
    SDword src[3];
    SDword dst[4];
    for (int i = 0; i < 3; ++i) { src[i].m_value = 0xA0u + i; }
    dst[3].m_value = 0x55u;
    SAllocator al;
    STag tag;
    SDword* r = UninitCopy(src, src + 3, dst, al, tag);
    bool ok = r == dst + 3 && g_constructs == 3 && dst[3].m_value == 0x55u;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_value != 0xA0u + k) ok = false;
    if (UninitCopy(src + 1, src + 1, dst, al, tag) != dst || g_constructs != 3) ok = false;
    if (!ok) { std::printf("00c29340_TEST FAIL\n"); return 1; }
    std::printf("00c29340_TEST PASS\n");
    return 0;
}