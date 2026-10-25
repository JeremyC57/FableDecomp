// Behaviour test for 0x0049B480 vector<CTexture>::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CTexture {
    void* PD3DTexture;
    unsigned int ByteLength : 28;
    unsigned int AllocationSource : 4;
    CTexture(int v) : PD3DTexture(0), ByteLength(v), AllocationSource(1) { ++g_live; }
    CTexture(const CTexture& o) : PD3DTexture(o.PD3DTexture), ByteLength(o.ByteLength), AllocationSource(o.AllocationSource) { ++g_live; }
    ~CTexture() { --g_live; }
};

struct Vec {
    CTexture* start; CTexture* finish; CTexture* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CTexture* pos, const CTexture& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CTexture* ns = len ? (CTexture*)std::malloc(len * sizeof(CTexture)) : 0;
        CTexture* nf = ns;
        for (CTexture* p = start; p != pos; ++p, ++nf) new (nf) CTexture(*p);
        if (fill_len == 1) { new (nf) CTexture(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CTexture(x);
        if (!atend) for (CTexture* p = pos; p != finish; ++p, ++nf) new (nf) CTexture(*p);
        for (CTexture* p = start; p != finish; ++p) p->~CTexture();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    CTexture a(5);
    v.insert_overflow(v.finish, a, 1, true);             // push_back into empty: len = 0 + max(0,1) = 1
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].ByteLength != 5) ok = 0;
    CTexture b(7);
    v.insert_overflow(v.finish, b, 1, true);             // len = 1 + 1 = 2
    if (v.size() != 2 || v.eos - v.start != 2 || v.start[1].ByteLength != 7) ok = 0;
    CTexture c(9);
    v.insert_overflow(v.start + 1, c, 3, false);         // insert 3 in the middle: len = 2 + 3 = 5
    if (v.size() != 5 || v.eos - v.start != 5) ok = 0;
    unsigned int want[5] = { 5, 9, 9, 9, 7 };
    for (int i = 0; i < 5; ++i) if (v.start[i].ByteLength != want[i] || v.start[i].AllocationSource != 1) ok = 0;
    if (g_live != 3 + 5) ok = 0;
    for (CTexture* p = v.start; p != v.finish; ++p) p->~CTexture();
    std::free(v.start);
    if (g_live != 3) ok = 0;
    if (!ok) { std::printf("0049b480_TEST FAIL\n"); return 1; }
    std::printf("0049b480_TEST PASS\n");
    return 0;
}