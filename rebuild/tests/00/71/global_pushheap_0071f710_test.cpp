// behaviour test for 0x0071f710 __push_heap<pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess>
#include <cstdio>
class CCompositeBlendDesc { public: unsigned int TextureIndex; int BlendType; unsigned char Diffuse[4]; unsigned char Specular[4]; };
class CTextureMorphEntry { public: int TextureLayer; CCompositeBlendDesc Blend; };
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
        v.first = keys[i]; v.second.TextureLayer = i; v.second.Blend.TextureIndex = 10u * i; v.second.Blend.BlendType = -i; for (int b = 0; b < 4; ++b) { v.second.Blend.Diffuse[b] = (unsigned char)(i + b); v.second.Blend.Specular[b] = (unsigned char)(2 * i + b); }
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("0071f710_TEST FAIL\n"); return 1; }
    }
    if (!(heap[0].first == 11 && heap[0].second.TextureLayer == 2 && heap[0].second.Blend.TextureIndex == 20u && heap[0].second.Blend.BlendType == -2 && heap[0].second.Blend.Diffuse[3] == 5 && heap[0].second.Blend.Specular[1] == 5)) { std::printf("0071f710_TEST FAIL\n"); return 1; }
    std::printf("0071f710_TEST PASS\n");
    return 0;
}