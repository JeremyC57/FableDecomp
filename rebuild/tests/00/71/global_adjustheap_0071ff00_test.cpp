// behaviour test for 0071ff00: SGI __adjust_heap over (hero morph type, texture morph entry with blend description) pairs by morph type
#include <cstdio>
#include <cstring>

enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct CCompositeBlendDesc {
    unsigned long TextureIndex; ECompositeBlendType BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour;
    CCompositeBlendDesc() { }
    CCompositeBlendDesc(const CCompositeBlendDesc& o) : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { }
};
struct CTextureMorphEntry { long TextureLayer; CCompositeBlendDesc Blend; };
struct CMorphPair { EHeroMorphType first; CTextureMorphEntry second; };
struct CMorphLess { bool operator()(const CMorphPair& a, const CMorphPair& b) const { return a.first < b.first; } };
typedef CMorphPair T;
typedef CMorphLess C;
static C MakePred() { C c; return c; }
static void Fill(T& t, int key, long i) { t.first = (EHeroMorphType)key; t.second.TextureLayer = key + 5; t.second.Blend.TextureIndex = (unsigned long)(key + 9); t.second.Blend.BlendType = (ECompositeBlendType)key; t.second.Blend.DiffuseColour.Set(1, 2, (unsigned char)(key & 0xff), 4); t.second.Blend.SpecularColour.Set(5, 6, 7, 8); }
static int Key(const T& t) { return (int)t.first; }
static bool Intact(const T& t, const int*) { int k = Key(t); return t.second.TextureLayer == k + 5 && t.second.Blend.TextureIndex == (unsigned long)(k + 9) && (int)t.second.Blend.BlendType == k && t.second.Blend.DiffuseColour.B == (unsigned char)(k & 0xff) && t.second.Blend.SpecularColour.A == 8; }

static void PushHeap(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void AdjustHeap(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
int main()
{
    static const int keys[11] = { 7, -2, 15, 3, 3, 0, 42, -9, 11, 5, 8 };
    T a[11];
    C pred = MakePred();
    long i = 0;
    long n = 11;
    long last = 0;
    for (i = 0; i < n; ++i) Fill(a[i], keys[i], i);
    for (i = n / 2; 0 < i; ) { --i; T v = a[i]; AdjustHeap(a, i, n, v, pred); }
    int ok = 1;
    for (i = 1; i < n; ++i) if (pred(a[0], a[i])) ok = 0;
    for (last = n; 1 < last; --last) {
        T value = a[last - 1];
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, value, pred);
    }
    for (i = 1; i < n; ++i) if (pred(a[i], a[i - 1])) ok = 0;
    for (i = 0; i < n; ++i) if (!Intact(a[i], keys)) ok = 0;
    if (Key(a[0]) != -9 || Key(a[n - 1]) != 42) ok = 0;

    if (!ok) { std::printf("0071ff00_TEST FAIL\n"); return 1; }
    std::printf("0071ff00_TEST PASS\n");
    return 0;
}