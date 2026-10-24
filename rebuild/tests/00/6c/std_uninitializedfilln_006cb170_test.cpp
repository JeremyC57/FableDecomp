// Behaviour test for 0x006CB170 __uninitialized_fill_n<CEngineTextureMorph*, unsigned int, CEngineTextureMorph>: n
// raw records each get the two index words and their own deep copy of x's blends; the end pointer is returned and
// n == 0 returns first untouched.
#include <cstdio>
#include <cstdlib>
#include <new>
#include <cstring>

struct CCompositeBlendDesc { unsigned int TextureIndex; int BlendType; unsigned int Diffuse; unsigned int Specular; };

struct BlendVector {
    CCompositeBlendDesc* Start;
    CCompositeBlendDesc* Finish;
    CCompositeBlendDesc* End;
    BlendVector() : Start(0), Finish(0), End(0) {}
    BlendVector(const BlendVector& x) : Start(0), Finish(0), End(0)
    {
        unsigned int n = (unsigned int)(x.Finish - x.Start);
        Start = n != 0 ? (CCompositeBlendDesc*)std::malloc(n * sizeof(CCompositeBlendDesc)) : 0;
        Finish = Start;
        End = Start + n;
        for (const CCompositeBlendDesc* p = x.Start; p != x.Finish; ++p) *Finish++ = *p;
    }
    ~BlendVector() { if (Start) std::free(Start); }
};

struct CEngineTextureMorph {
    int         OriginalTextureIndex;
    int         MorphID;
    BlendVector Blends;
};

static CEngineTextureMorph* UninitializedFillN(CEngineTextureMorph* first, unsigned int n, const CEngineTextureMorph& x)
{
    CEngineTextureMorph* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur) new (cur) CEngineTextureMorph(x);
    return cur;
}

int main()
{
    CEngineTextureMorph x;
    x.OriginalTextureIndex = 12; x.MorphID = 5;
    x.Blends.Start = (CCompositeBlendDesc*)std::malloc(2 * sizeof(CCompositeBlendDesc));
    x.Blends.Finish = x.Blends.End = x.Blends.Start + 2;
    x.Blends.Start[0].TextureIndex = 1; x.Blends.Start[1].TextureIndex = 2;

    CEngineTextureMorph* raw = (CEngineTextureMorph*)std::malloc(3 * sizeof(CEngineTextureMorph));
    std::memset(raw, 0xcd, 3 * sizeof(CEngineTextureMorph));
    CEngineTextureMorph* end = UninitializedFillN(raw, 3, x);
    if (end != raw + 3) { std::printf("006cb170_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) {
        if (raw[i].OriginalTextureIndex != 12 || raw[i].MorphID != 5 || raw[i].Blends.Finish - raw[i].Blends.Start != 2
            || raw[i].Blends.End - raw[i].Blends.Start != 2 || raw[i].Blends.Start == x.Blends.Start
            || raw[i].Blends.Start[1].TextureIndex != 2) { std::printf("006cb170_TEST FAIL\n"); return 1; }
    }
    if (raw[0].Blends.Start == raw[1].Blends.Start) { std::printf("006cb170_TEST FAIL\n"); return 1; }
    if (UninitializedFillN(raw, 0, x) != raw) { std::printf("006cb170_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) raw[i].~CEngineTextureMorph();
    std::free(raw);
    std::printf("006cb170_TEST PASS\n");
    return 0;
}