// Behaviour test for 0x00BF94D0 std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_dtors = 0;
class CCharString { public:                       // counts live strings
    int id;
    CCharString(int i) : id(i) { }
    CCharString(const CCharString& o) : id(o.id) { }
    ~CCharString() { ++g_dtors; }
};
struct CPaletteEntry {                  // {Index, Name}
    unsigned long Index; CCharString Name;
    CPaletteEntry(int s, int o, int) : Index(s), Name(o) { }
    CPaletteEntry(const CPaletteEntry& o) : Index(o.Index), Name(o.Name) { }
    int info() const { return Name.id; }
};

struct Vec {
    CPaletteEntry* start; CPaletteEntry* finish; CPaletteEntry* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CPaletteEntry* pos, const CPaletteEntry& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CPaletteEntry* ns = len ? (CPaletteEntry*)std::malloc(len * sizeof(CPaletteEntry)) : 0;
        CPaletteEntry* nf = ns;
        for (CPaletteEntry* p = start; p != pos; ++p, ++nf) new (nf) CPaletteEntry(*p);
        if (fill_len == 1) { new (nf) CPaletteEntry(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CPaletteEntry(x);
        if (!atend) for (CPaletteEntry* p = pos; p != finish; ++p, ++nf) new (nf) CPaletteEntry(*p);
        for (CPaletteEntry* p = start; p != finish; ++p) p->~CPaletteEntry();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CPaletteEntry a(1, 2, 3), b(4, 5, 6);
    v.insert_overflow(v.finish, a, 1, true);           // push_back into an empty vector
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].info() != 2 || g_dtors != 0) { std::printf("00bf94d0_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start, b, 2, false);           // two copies in front: len = 1 + 2
    if (v.size() != 3 || v.start[0].Index != 4 || v.start[1].info() != 5 || v.start[2].Index != 1 || g_dtors != 1) { std::printf("00bf94d0_TEST FAIL\n"); return 1; }
    for (CPaletteEntry* p = v.start; p != v.finish; ++p) p->~CPaletteEntry();
    std::free(v.start);
    std::printf("00bf94d0_TEST PASS\n");
    return 0;
}