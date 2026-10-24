// Behaviour test for 0x006E1F40 __copy_ptrs over pair<unsigned long, CParticleUpdateParamDesc>: (last - first) elements
// are assigned in order (key and the two payload words; the polymorphic desc keeps its own vtable), the advanced
// result is returned, and an empty range copies nothing.
#include <cstdio>

struct CBase { virtual ~CBase() {} virtual int Kind() const { return 0; } };
struct Desc : CBase { void* m_pName; void* m_pParam; };
struct Entry { unsigned int first; Desc second; };

static Entry* CopyPtrs(const Entry* first, const Entry* last, Entry* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}

int main()
{
    Entry src[3], dst[4];
    int names[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 10 + i; src[i].second.m_pName = &names[i]; src[i].second.m_pParam = 0; }
    for (int i = 0; i < 4; ++i) { dst[i].first = 0; dst[i].second.m_pName = 0; dst[i].second.m_pParam = 0; }
    Entry* end = CopyPtrs(src, src + 3, dst);
    if (end != dst + 3 || dst[0].first != 10 || dst[2].first != 12 || dst[1].second.m_pName != &names[1] || dst[3].first != 0) {
        std::printf("006e1f40_TEST FAIL\n"); return 1;
    }
    if (CopyPtrs(src, src, dst + 3) != dst + 3 || dst[3].first != 0) { std::printf("006e1f40_TEST FAIL\n"); return 1; }
    std::printf("006e1f40_TEST PASS\n");
    return 0;
}