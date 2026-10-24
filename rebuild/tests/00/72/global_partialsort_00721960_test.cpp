// Behaviour test for 0x00721960 __partial_sort over pair<EHeroMorphType,CTextureMorphEntry>
// (__make_heap / __pop_heap / sort_heap callees modelled with their SGI bodies).
#include <cstdio>
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() { }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct CCompositeBlendDesc {
    unsigned long TextureIndex;
    long BlendType;
    CRGBColour DiffuseColour;
    CRGBColour SpecularColour;
    CCompositeBlendDesc() { }
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { }
};
struct CTextureMorphEntry { long TextureLayer; CCompositeBlendDesc Blend; };
struct CMorphPair { EHeroMorphType first; CTextureMorphEntry second; };
struct CMorphLess {
    char m_tag;
    bool operator()(const CMorphPair& a, const CMorphPair& b) const { return a.first < b.first; }
};
typedef CMorphPair T;
static void push_heap_(T* first, int hole, int top, T value, CMorphLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void adjust_heap(T* first, int hole, int len, T value, CMorphLess comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1]))
            child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) {
        first[hole] = first[child - 1];
        hole = child - 1;
    }
    push_heap_(first, hole, top, value, comp);
}
static void make_heap_(T* first, T* last, CMorphLess comp)
{
    if (last - first < 2)
        return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
static void pop_heap_(T* first, T* last, T* result, T value, CMorphLess comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, (int)(last - first), value, comp);
}
static void sort_heap_(T* first, T* last, CMorphLess comp)
{
    while (last - first > 1) {
        --last;
        pop_heap_(first, last, last, T(*last), comp, (int*)0);
    }
}
static void partial_sort_(T* first, T* middle, T* last, T*, CMorphLess comp)
{
    make_heap_(first, middle, comp);
    for (T* i = middle; i < last; ++i)
        if (comp(*i, *first))
            pop_heap_(first, middle, i, T(*i), comp, (int*)0);
    sort_heap_(first, middle, comp);
}
int main()
{
    T a[9];
    int keys[9] = { 30, 5, 22, 17, 2, 40, 9, 13, 1 };
    for (int i = 0; i < 9; ++i) {
        a[i].first = (EHeroMorphType)keys[i];
        a[i].second.TextureLayer = keys[i] * 2;
        a[i].second.Blend.TextureIndex = keys[i];
        a[i].second.Blend.BlendType = -keys[i];
        a[i].second.Blend.DiffuseColour.Set((unsigned char)keys[i], 1, 2, 3);
        a[i].second.Blend.SpecularColour.Set(4, 5, 6, (unsigned char)keys[i]);
    }
    CMorphLess comp;
    comp.m_tag = 0;
    partial_sort_(a, a + 3, a + 9, (T*)0, comp);
    int want[3] = { 1, 2, 5 };
    for (int i = 0; i < 3; ++i) {
        const T& e = a[i];
        if ((int)e.first != want[i] || e.second.TextureLayer != want[i] * 2 || e.second.Blend.BlendType != -want[i]
            || e.second.Blend.DiffuseColour.R != want[i] || e.second.Blend.SpecularColour.A != want[i] || e.second.Blend.SpecularColour.G != 5) {
            std::printf("00721960_TEST FAIL\n");
            return 1;
        }
    }
    for (int i = 3; i < 9; ++i)
        if ((int)a[i].first < 5) { std::printf("00721960_TEST FAIL\n"); return 1; }
    std::printf("00721960_TEST PASS\n");
    return 0;
}