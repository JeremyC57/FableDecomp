// behaviour test for 0x00819b10 _Push_heap<pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess>
#include <cstdio>
struct CTextureMorphEntry { int MorphType; int TextureLayer; int ReplacingTextureIndex; int BankIndex; int Blend; };
struct TEntry { int first; CTextureMorphEntry second; };
struct TCompare { char m_tag; bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; } };
typedef TEntry E;
typedef TCompare C;
static void _Push_heap(E* first, long hole, long top, E value, C comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}

int main()
{
    static const int keys[7] = { 4, -3, 11, 0, 7, -8, 5 };
    E heap[7];
    C c;
    for (int i = 0; i < 7; ++i) {
        E v;
        v.first = keys[i]; v.second.MorphType = keys[i]; v.second.TextureLayer = i; v.second.ReplacingTextureIndex = 2 * i; v.second.BankIndex = 3 * i; v.second.Blend = 4 * i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00819b10_TEST FAIL\n"); return 1; }
    }
    if (!(heap[0].first == 11 && heap[0].second.TextureLayer == 2 && heap[0].second.ReplacingTextureIndex == 4 && heap[0].second.BankIndex == 6 && heap[0].second.Blend == 8)) { std::printf("00819b10_TEST FAIL\n"); return 1; }
    std::printf("00819b10_TEST PASS\n");
    return 0;
}