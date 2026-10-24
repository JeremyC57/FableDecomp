// Behaviour test for _STL::__unguarded_partition<CGroup*, CGroup, comp> @ 0x00AD2AF0
// Hoare partition around a by-value pivot: returns the cut such that comp(pivot, x) is false left of it
// and comp(x, pivot) is false right of it.
#include <cstdio>

struct CGroup { unsigned int BoneIndex; float BoneWeight; };
typedef bool (*TGroupCompare)(const CGroup& a, const CGroup& b);

static bool HeavierFirst(const CGroup& a, const CGroup& b) { return a.BoneWeight > b.BoneWeight; }

static CGroup* unguarded_partition(CGroup* first, CGroup* last, CGroup pivot, TGroupCompare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        CGroup tmp = *first;
        *first = *last;
        *last = tmp;
        ++first;
    }
}

int main()
{
    CGroup g[8] = { { 0, 0.3f }, { 1, 0.9f }, { 2, 0.1f }, { 3, 0.5f }, { 4, 0.7f }, { 5, 0.2f }, { 6, 0.8f }, { 7, 0.4f } };
    CGroup pivot = g[3];                                  // weight 0.5
    CGroup* cut = unguarded_partition(g, g + 8, pivot, HeavierFirst);
    int ok = cut > g && cut < g + 8;
    for (CGroup* p = g; p < cut && ok; ++p) if (HeavierFirst(pivot, *p)) ok = 0;
    for (CGroup* p = cut; p < g + 8 && ok; ++p) if (HeavierFirst(*p, pivot)) ok = 0;
    unsigned int seen = 0;
    for (int i = 0; i < 8; ++i) seen |= 1u << g[i].BoneIndex;
    if (seen != 0xFFu) ok = 0;
    if (!ok) { std::printf("00ad2af0_TEST FAIL\n"); return 1; }
    std::printf("00ad2af0_TEST PASS\n");
    return 0;
}