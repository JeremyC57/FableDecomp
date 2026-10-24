// behaviour test for 0x0067e650 _Push_heap (CDegradableInfo, `>` on HealthPercentage)
#include <cstdio>
#include <cstring>
struct CEngineGraphic {   // PDB: 14 bytes of data, 16 with the dword alignment
    unsigned long BankIndex;      // +0x0
    float AnimStep;               // +0x4
    float RenderSizeX;            // +0x8
    bool AdditiveAlpha;           // +0xc
    unsigned char Type;           // +0xd
};
struct CDegradableInfo {
    float HealthPercentage;
    CEngineGraphic EngineGraphic;
    int SmashParticleEmitter;
    bool BlocksNavigation;
};
struct CIsHealth1GTHealth2 {
    char m_tag;
    bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
};
static void _Push_heap(CDegradableInfo* first, int hole, int top, CDegradableInfo value, CIsHealth1GTHealth2 comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static int check(CDegradableInfo* h, int n)
{
    CIsHealth1GTHealth2 c;
    for (int i = 1; i < n; ++i)
        if (c(h[(i - 1) / 2], h[i])) return 0;   // heap property: no parent orders before its child
    return 1;
}
int main()
{
    static const int keys[8] = { 5, 3, 9, 1, 7, 2, 8, 4 };
    CDegradableInfo heap[8];
    std::memset(heap, 0, sizeof(heap));
    CIsHealth1GTHealth2 c;
    c.m_tag = 0;
    for (int i = 0; i < 8; ++i) {
        CDegradableInfo v;
        std::memset(&v, 0, sizeof(v));
        v.HealthPercentage = (float)keys[i];
        v.SmashParticleEmitter = i + 1;   // payload marker travels with the key
        _Push_heap(heap, i, 0, v, c);
        if (!check(heap, i + 1)) { std::printf("0067e650_TEST FAIL\n"); return 1; }
    }
    // the top is the greatest key under `<`, the smallest under `>`
    if (heap[0].HealthPercentage != (float)(1)) { std::printf("0067e650_TEST FAIL\n"); return 1; }
    if (heap[0].SmashParticleEmitter != 4) { std::printf("0067e650_TEST FAIL\n"); return 1; }
    // a push with top == hole stores in place
    CDegradableInfo w;
    std::memset(&w, 0, sizeof(w));
    w.HealthPercentage = (float)100;
    _Push_heap(heap, 7, 7, w, c);
    if (heap[7].HealthPercentage != (float)100) { std::printf("0067e650_TEST FAIL\n"); return 1; }
    std::printf("0067e650_TEST PASS\n");
    return 0;
}