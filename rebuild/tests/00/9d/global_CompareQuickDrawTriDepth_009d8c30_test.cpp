// Behaviour test for CompareQuickDrawTriDepth (0x009D8C30): back-to-front qsort comparator.
#include <cstdio>
#include <cstdlib>

class CQuickDrawTriInfo
{ public:
    unsigned char _pad00[0x24];
    float         m_depth;
    unsigned char _pad28[0x3C - 0x28];
};

int CompareQuickDrawTriDepth(const void* left, const void* right)
{
    const CQuickDrawTriInfo* a = (const CQuickDrawTriInfo*)left;
    const CQuickDrawTriInfo* b = (const CQuickDrawTriInfo*)right;
    return a->m_depth > b->m_depth ? -1 : 1;
}

int main()
{
    if (sizeof(CQuickDrawTriInfo) != 0x3C) { std::printf("009d8c30_TEST FAIL\n"); return 1; }
    CQuickDrawTriInfo r[4];
    float d[4] = { 2.0f, 9.0f, -1.0f, 5.0f };
    for (int i = 0; i < 4; ++i) r[i].m_depth = d[i];
    if (CompareQuickDrawTriDepth(&r[1], &r[0]) != -1) { std::printf("009d8c30_TEST FAIL\n"); return 1; }
    if (CompareQuickDrawTriDepth(&r[0], &r[1]) != 1) { std::printf("009d8c30_TEST FAIL\n"); return 1; }
    if (CompareQuickDrawTriDepth(&r[0], &r[0]) != 1) { std::printf("009d8c30_TEST FAIL\n"); return 1; }
    std::qsort(r, 4, sizeof(CQuickDrawTriInfo), CompareQuickDrawTriDepth);
    if (r[0].m_depth != 9.0f || r[1].m_depth != 5.0f || r[2].m_depth != 2.0f || r[3].m_depth != -1.0f) { std::printf("009d8c30_TEST FAIL\n"); return 1; }
    std::printf("009d8c30_TEST PASS\n");
    return 0;
}