// Behaviour test for 0x00720710 std::__make_heap over pair<long, CTCHeroMorph::CTextureLayer> by key (the SGI __adjust_heap / __push_heap pair stands in for 0x0071FF00).
#include <cstdio>

enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0, COMPOSITE_BLEND_ADD = 2 };
class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CCompositeBlendDesc { public: unsigned long TextureIndex; ECompositeBlendType BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour; };
struct CTextureLayer { long TextureID; CCompositeBlendDesc Blend; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<long, CTextureLayer> T;
typedef CKeyPairCompareLess<long, CTextureLayer> P;

static int g_adjusts = 0;
static void __push_heap(T* first, int holeIndex, int topIndex, T value, P comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void __adjust_heap(T* first, int holeIndex, int len, T value, P comp)
{
    ++g_adjusts;
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
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
    __push_heap(first, holeIndex, topIndex, value, comp);
}
static void __make_heap(T* first, T* last, P comp, T*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

int main()
{
    long keys[7] = { -4, 11, 3, 11, 0, 25, -9 };
    T a[7];
    for (int k = 0; k < 7; ++k) {
        a[k].first = keys[k];
        a[k].second.TextureID = 100 + k;
        a[k].second.Blend.TextureIndex = k;
        a[k].second.Blend.BlendType = (k & 1) ? COMPOSITE_BLEND_ADD : COMPOSITE_BLEND_NONE;
        a[k].second.Blend.DiffuseColour.Set((unsigned char)(10 + k), (unsigned char)(20 + k), (unsigned char)(30 + k), 255);
        a[k].second.Blend.SpecularColour.Set((unsigned char)k, 1, 2, (unsigned char)(200 + k));
    }
    P comp;
    __make_heap(a, a + 7, comp, (T*)0, (int*)0);
    if (g_adjusts != 3) { std::printf("00720710_TEST FAIL\n"); return 1; }
    for (int k = 1; k < 7; ++k)
        if (a[(k - 1) / 2].first < a[k].first) { std::printf("00720710_TEST FAIL\n"); return 1; }
    if (a[0].first != 25) { std::printf("00720710_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 7; ++k) {
        long s = a[k].second.Blend.TextureIndex;
        const CCompositeBlendDesc& b = a[k].second.Blend;
        if (keys[s] != a[k].first || a[k].second.TextureID != 100 + s || b.DiffuseColour.R != 10 + s || b.DiffuseColour.G != 20 + s
            || b.DiffuseColour.B != 30 + s || b.DiffuseColour.A != 255 || b.SpecularColour.R != s || b.SpecularColour.A != 200 + s
            || b.BlendType != ((s & 1) ? COMPOSITE_BLEND_ADD : COMPOSITE_BLEND_NONE)) { std::printf("00720710_TEST FAIL\n"); return 1; }
    }
    std::printf("00720710_TEST PASS\n");
    return 0;
}