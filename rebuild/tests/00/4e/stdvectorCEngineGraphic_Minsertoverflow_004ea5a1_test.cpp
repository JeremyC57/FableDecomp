// Behaviour test for 0x004EA5A1 vector<CEngineGraphic>::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

#pragma pack(push, 1)
class CEngineGraphic { public:
    unsigned int  ByteLength;      // BankIndex
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char AllocationSource; // Type
};
#pragma pack(pop)
static int g_live = 0;
static CEngineGraphic MakeG(int v) { CEngineGraphic g; g.ByteLength = v; g.AnimStep = 0.5f; g.RenderSizeX = 1.0f; g.AdditiveAlpha = true; g.AllocationSource = 1; return g; }

struct Vec {
    CEngineGraphic* start; CEngineGraphic* finish; CEngineGraphic* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CEngineGraphic* pos, const CEngineGraphic& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CEngineGraphic* ns = len ? (CEngineGraphic*)std::malloc(len * sizeof(CEngineGraphic)) : 0;
        CEngineGraphic* nf = ns;
        for (CEngineGraphic* p = start; p != pos; ++p, ++nf) new (nf) CEngineGraphic(*p);
        if (fill_len == 1) { new (nf) CEngineGraphic(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CEngineGraphic(x);
        if (!atend) for (CEngineGraphic* p = pos; p != finish; ++p, ++nf) new (nf) CEngineGraphic(*p);
        
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    int ok = 1;
    if (sizeof(CEngineGraphic) != 14) ok = 0;
    Vec v; v.start = v.finish = v.eos = 0;
    CEngineGraphic a = MakeG(5);
    v.insert_overflow(v.finish, a, 1, true);             // push_back into empty: len = 0 + max(0,1) = 1
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].ByteLength != 5) ok = 0;
    CEngineGraphic b = MakeG(7);
    v.insert_overflow(v.finish, b, 1, true);             // len = 1 + 1 = 2
    if (v.size() != 2 || v.eos - v.start != 2 || v.start[1].ByteLength != 7) ok = 0;
    CEngineGraphic c = MakeG(9);
    v.insert_overflow(v.start + 1, c, 3, false);         // insert 3 in the middle: len = 2 + 3 = 5
    if (v.size() != 5 || v.eos - v.start != 5) ok = 0;
    unsigned int want[5] = { 5, 9, 9, 9, 7 };
    for (int i = 0; i < 5; ++i) if (v.start[i].ByteLength != want[i] || v.start[i].AllocationSource != 1 || !v.start[i].AdditiveAlpha) ok = 0;
    
    
    std::free(v.start);
    
    if (!ok) { std::printf("004ea5a1_TEST FAIL\n"); return 1; }
    std::printf("004ea5a1_TEST PASS\n");
    return 0;
}