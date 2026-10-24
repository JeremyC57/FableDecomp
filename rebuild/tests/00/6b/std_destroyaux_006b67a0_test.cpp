// Behaviour test for 0x006B67A0 std::__destroy_aux(CBlendedEnvironmentTheme*, CBlendedEnvironmentTheme*, const __false_type&).
#include <cstdio>

static void* g_freed[64];
static int g_nfreed = 0;
static void RecordFree(void* p) { g_freed[g_nfreed++] = p; }

struct FalseType { FalseType() { } };

template <class T>
struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CArray() { if (_M_start != 0) RecordFree(_M_start); }
};

struct CRGBFloatColour { float R, G, B, A; };

struct CIDrawEnvironmentTextureSet {
    CArray<int>   Textures;
    CArray<float> Blends;
};

struct CBlendedEnvironmentTheme {
    float TimeOfDay;
    CRGBFloatColour             Colours[19];
    float                       Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
};

template <class T> inline void DestroyOne(T* p) { p->~T(); }

template <class ForwardIter>
void DestroyAux(ForwardIter first, ForwardIter last, const FalseType&)
{
    for (; first != last; ++first)
        DestroyOne(&*first);
}

static int ints[16];
static float floats[16];
static unsigned char raw[sizeof(CBlendedEnvironmentTheme) * 2];

int main()
{
    CBlendedEnvironmentTheme* p = (CBlendedEnvironmentTheme*)raw;
    int k = 0;
    for (int e = 0; e < 2; ++e)
        for (int s = 0; s < 3; ++s) {
            CIDrawEnvironmentTextureSet& t = p[e].TextureSets[s];
            t.Textures._M_start = (s == 1 && e == 0) ? 0 : &ints[k];      // one empty vector: no free
            t.Blends._M_start = &floats[k];
            ++k;
        }
    DestroyAux(p, p + 2, FalseType());
    // reverse member order within each element: set 2 (Blends, Textures), set 1 (Blends only), set 0 ...
    void* expect[] = { &floats[2], &ints[2], &floats[1], &floats[0], &ints[0],
                       &floats[5], &ints[5], &floats[4], &ints[4], &floats[3], &ints[3] };
    int ok = g_nfreed == 11;
    for (int i = 0; ok && i < 11; ++i)
        if (g_freed[i] != expect[i]) ok = 0;
    g_nfreed = 0;
    DestroyAux(p, p, FalseType());                       // empty range
    if (g_nfreed != 0) ok = 0;
    if (!ok) { std::printf("006b67a0_TEST FAIL\n"); return 1; }
    std::printf("006b67a0_TEST PASS\n");
    return 0;
}