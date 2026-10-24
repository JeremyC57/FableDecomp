// Behaviour test for 0x006CB390 vector<CEngineTextureMorph>::_M_insert_overflow: the buffer grows to
// size + max(size, n); the prefix is copied, one copy (n == 1) or n copies of x are constructed, the suffix is copied
// unless atend, the old records are cleared (their blends and the buffer freed) and start/finish/end are reset.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_blendFrees = 0;

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
};

struct CEngineTextureMorph {
    int         OriginalTextureIndex;
    int         MorphID;
    BlendVector Blends;
};

static CEngineTextureMorph* UninitCopy(CEngineTextureMorph* first, CEngineTextureMorph* last, CEngineTextureMorph* result)
{
    CEngineTextureMorph* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CEngineTextureMorph(*first);
    return cur;
}

static CEngineTextureMorph* UninitFillN(CEngineTextureMorph* first, unsigned int n, const CEngineTextureMorph& x)
{
    CEngineTextureMorph* cur = first;
    for (; n > 0; --n, ++cur) new (cur) CEngineTextureMorph(x);
    return cur;
}

struct MorphVector {
    CEngineTextureMorph* Start;
    CEngineTextureMorph* Finish;
    CEngineTextureMorph* End;
    void Clear()                                         // the out-of-line _M_clear
    {
        for (CEngineTextureMorph* p = Start; p != Finish; ++p)
            if (p->Blends.Start) { ++g_blendFrees; std::free(p->Blends.Start); }
        if (Start) std::free(Start);
    }
    void InsertOverflow(CEngineTextureMorph* position, const CEngineTextureMorph& x, unsigned int fillLen, bool atend)
    {
        unsigned int oldSize = (unsigned int)(Finish - Start);
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        CEngineTextureMorph* newStart = len != 0 ? (CEngineTextureMorph*)std::malloc(len * sizeof(CEngineTextureMorph)) : 0;
        CEngineTextureMorph* newFinish = UninitCopy(Start, position, newStart);
        if (fillLen == 1) {
            if (newFinish) new (newFinish) CEngineTextureMorph(x);
            ++newFinish;
        } else
            newFinish = UninitFillN(newFinish, fillLen, x);
        if (!atend)
            newFinish = UninitCopy(position, Finish, newFinish);
        Clear();
        Start = newStart; Finish = newFinish; End = newStart + len;
    }
};

static CEngineTextureMorph Make(int index, int blends)
{
    CEngineTextureMorph m;
    m.OriginalTextureIndex = index; m.MorphID = index * 10;
    if (blends) {
        m.Blends.Start = (CCompositeBlendDesc*)std::malloc(blends * sizeof(CCompositeBlendDesc));
        m.Blends.Finish = m.Blends.End = m.Blends.Start + blends;
        for (int i = 0; i < blends; ++i) m.Blends.Start[i].TextureIndex = index * 100 + i;
    }
    return m;
}

int main()
{
    MorphVector v; v.Start = v.Finish = v.End = 0;
    CEngineTextureMorph a = Make(1, 1);
    v.InsertOverflow(v.Finish, a, 1, true);              // empty: grow to 1
    if (v.Finish - v.Start != 1 || v.End - v.Start != 1 || v.Start[0].OriginalTextureIndex != 1
        || v.Start[0].Blends.Start == a.Blends.Start || v.Start[0].Blends.Start[0].TextureIndex != 100) { std::printf("006cb390_TEST FAIL\n"); return 1; }
    CEngineTextureMorph b = Make(2, 2);
    v.InsertOverflow(v.Finish, b, 1, true);              // grow to 2: old record cleared (one blends buffer freed)
    if (v.Finish - v.Start != 2 || v.End - v.Start != 2 || g_blendFrees != 1 || v.Start[1].MorphID != 20
        || v.Start[1].Blends.Finish - v.Start[1].Blends.Start != 2) { std::printf("006cb390_TEST FAIL\n"); return 1; }
    CEngineTextureMorph c = Make(3, 0);
    v.InsertOverflow(v.Start + 1, c, 3, false);           // insert 3 copies in the middle: grow to 2 + 3 = 5
    if (v.Finish - v.Start != 5 || v.End - v.Start != 5 || g_blendFrees != 3
        || v.Start[0].OriginalTextureIndex != 1 || v.Start[1].OriginalTextureIndex != 3 || v.Start[3].OriginalTextureIndex != 3
        || v.Start[4].OriginalTextureIndex != 2 || v.Start[4].Blends.Start[1].TextureIndex != 201) { std::printf("006cb390_TEST FAIL\n"); return 1; }
    v.Clear();
    std::free(a.Blends.Start); std::free(b.Blends.Start);
    std::printf("006cb390_TEST PASS\n");
    return 0;
}