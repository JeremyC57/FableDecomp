// Behaviour test for 0x006CB010 vector<CCompositeBlendDesc>::push_back: with spare capacity the blend is copied in
// place; when full the buffer grows to size + max(size, 1), the old blends are copied over, the new one appended and
// the old buffer freed.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
static int g_mallocs = 0;
static int g_frees = 0;

struct CRGBColour { unsigned char B, G, R, A; };

struct CCompositeBlendDesc {
    unsigned int  TextureIndex;
    int           BlendType;
    CRGBColour    DiffuseColour;
    CRGBColour    SpecularColour;
    CCompositeBlendDesc() : TextureIndex(0), BlendType(0) { DiffuseColour.B = DiffuseColour.G = DiffuseColour.R = DiffuseColour.A = 0; SpecularColour = DiffuseColour; }
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { ++g_copies; }
};

static void* TestMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void TestFree(void* p) { ++g_frees; std::free(p); }

static CCompositeBlendDesc* UninitCopy(CCompositeBlendDesc* first, CCompositeBlendDesc* last, CCompositeBlendDesc* result)
{
    CCompositeBlendDesc* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CCompositeBlendDesc(*first);
    return cur;
}

struct BlendVector {
    CCompositeBlendDesc* Start;
    CCompositeBlendDesc* Finish;
    CCompositeBlendDesc* End;
    void push_back(const CCompositeBlendDesc& x)
    {
        if (Finish != End) {
            if (Finish) new (Finish) CCompositeBlendDesc(x);
            ++Finish;
            return;
        }
        unsigned int fill = 1;
        unsigned int oldSize = (unsigned int)(Finish - Start);
        unsigned int len = oldSize + (oldSize < fill ? fill : oldSize);
        CCompositeBlendDesc* mem = len != 0 ? (CCompositeBlendDesc*)TestMalloc(len * sizeof(CCompositeBlendDesc)) : 0;
        CCompositeBlendDesc* f = UninitCopy(Start, Finish, mem);
        if (f) new (f) CCompositeBlendDesc(x);
        ++f;
        if (Start) TestFree(Start);
        End = mem + len; Start = mem; Finish = f;
    }
};

int main()
{
    if (sizeof(CCompositeBlendDesc) != 16) { std::printf("006cb010_TEST FAIL\n"); return 1; }
    BlendVector v; v.Start = v.Finish = v.End = 0;
    CCompositeBlendDesc d;
    d.TextureIndex = 7; d.BlendType = 3; d.DiffuseColour.R = 0xff;
    v.push_back(d);                                    // empty: grow to 1
    if (v.Finish - v.Start != 1 || v.End - v.Start != 1 || g_mallocs != 1 || g_frees != 0 || g_copies != 1
        || v.Start[0].TextureIndex != 7 || v.Start[0].BlendType != 3 || v.Start[0].DiffuseColour.R != 0xff) { std::printf("006cb010_TEST FAIL\n"); return 1; }
    d.TextureIndex = 8; v.push_back(d);                // full: grow to 2, one copy over + one new
    d.TextureIndex = 9; v.push_back(d);                // full: grow to 4, two copies over + one new
    if (v.Finish - v.Start != 3 || v.End - v.Start != 4 || g_mallocs != 3 || g_frees != 2 || g_copies != 1 + 2 + 3
        || v.Start[0].TextureIndex != 7 || v.Start[1].TextureIndex != 8 || v.Start[2].TextureIndex != 9) { std::printf("006cb010_TEST FAIL\n"); return 1; }
    d.TextureIndex = 10; v.push_back(d);               // spare slot: in place, no allocation
    if (v.Finish - v.Start != 4 || g_mallocs != 3 || g_copies != 7 || v.Start[3].TextureIndex != 10) { std::printf("006cb010_TEST FAIL\n"); return 1; }
    std::free(v.Start);
    std::printf("006cb010_TEST PASS\n");
    return 0;
}