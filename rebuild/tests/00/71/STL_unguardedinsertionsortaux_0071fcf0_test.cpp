// Behaviour test for 0071fcf0 (_STL::__unguarded_insertion_sort_aux over pair<long, CTCHeroMorph::CTextureLayer> by CKeyPairCompareLess): the range comes out ordered by the comparator, every element
// moved whole (all members still agree with its key), and the by-value copies are balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
struct CCharString {                            // stands in for the engine string (copy 0x0099EC30, dtor 0x0099EAE0)
    const char* m_pData;
    explicit CCharString(int k) : m_pData(g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return m_pData[0] - 'a'; }
};
struct C3DVector {                              // memberwise user copy constructor, as retail
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
struct CRGBColour {                             // user copy constructor (R, G, B, A store order), as retail
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};
struct CCompositeBlendDesc {
    unsigned int TextureIndex;
    int          BlendType;
    CRGBColour   DiffuseColour;
    CRGBColour   SpecularColour;
    CCompositeBlendDesc(unsigned int t, int b, const CRGBColour& d, const CRGBColour& s) : TextureIndex(t), BlendType(b), DiffuseColour(d), SpecularColour(s) { }
};
struct CTextureLayer {
    long                TextureID;
    CCompositeBlendDesc Blend;
    CTextureLayer(long id, const CCompositeBlendDesc& b) : TextureID(id), Blend(b) { }
};
typedef pair<long, CTextureLayer> T;
struct C {                                      // CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer>
    bool operator()(const T& a, const T& b) const { return a.first < b.first; }
};
static T Make(int k)
{
    return T((long)k, CTextureLayer(k + 20, CCompositeBlendDesc((unsigned int)k * 2u, k + 1,
             CRGBColour((unsigned char)k, 1, 2, 3), CRGBColour(4, 5, (unsigned char)(k + 6), 7))));
}
static int KeyOf(const T& x) { return (int)x.first; }
static bool Consistent(const T& x) { return x.second.TextureID == x.first + 20 && x.second.Blend.TextureIndex == (unsigned int)x.first * 2u && x.second.Blend.BlendType == x.first + 1 && x.second.Blend.DiffuseColour.R == x.first && x.second.Blend.DiffuseColour.G == 1 && x.second.Blend.DiffuseColour.B == 2 && x.second.Blend.DiffuseColour.A == 3 && x.second.Blend.SpecularColour.R == 4 && x.second.Blend.SpecularColour.G == 5 && x.second.Blend.SpecularColour.B == x.first + 6 && x.second.Blend.SpecularColour.A == 7; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x0071F5E0)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)   // the function under test
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
#define FAIL() { std::printf("0071fcf0_TEST FAIL\n"); return 1; }
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };       // keys[0] is the minimum: the guard the unguarded insert relies on
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(Make(keys[i]));
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);   // an empty range changes nothing
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) FAIL();
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) FAIL();
    if (g_live != base + 0 * 8) FAIL();
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    if (g_live != base) FAIL();
    std::printf("0071fcf0_TEST PASS\n");
    return 0;
}