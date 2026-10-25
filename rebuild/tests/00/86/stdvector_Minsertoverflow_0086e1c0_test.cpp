// Behaviour test for 0x0086E1C0 vector<CVectorMap<ulong, CTextureMorph>>::_M_insert_overflow: max(2*size, size+fill_len) 16-byte maps; head, fill_len copies of x (deep vector copy + Dirty byte) and (unless atend) the tail; the old maps' blocks and the old buffer freed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct Morph { unsigned int key, NewTexture, GroupID; };
struct MorphMap {
    Morph* start; Morph* finish; Morph* eos; unsigned char Compare; bool Dirty;
    MorphMap() : start(0), finish(0), eos(0), Compare(0), Dirty(false) { }
    MorphMap(const MorphMap& o) : Compare(0), Dirty(o.Dirty)
    {
        unsigned int n = (unsigned int)(o.finish - o.start);
        start = n ? (Morph*)std::malloc(n * sizeof(Morph)) : 0;
        if (n) std::memcpy(start, o.start, n * sizeof(Morph));
        finish = start + n; eos = finish;
    }
};
struct Vec {
    MorphMap* start; MorphMap* finish; MorphMap* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear() { for (MorphMap* p = start; p != finish; ++p) if (p->start) std::free(p->start); if (start) std::free(start); }
    void insert_overflow(MorphMap* pos, const MorphMap& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        MorphMap* ns = len ? (MorphMap*)std::malloc(len * sizeof(MorphMap)) : 0;
        MorphMap* nf = ns;
        for (MorphMap* p = start; p != pos; ++p, ++nf) new (nf) MorphMap(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++nf) new (nf) MorphMap(x);
        if (!atend) for (MorphMap* p = pos; p != finish; ++p, ++nf) new (nf) MorphMap(*p);
        clear();
        start = ns; finish = nf; eos = ns + len;
    }
};
static int fail() { std::printf("0086e1c0_TEST FAIL\n"); return 1; }
int main()
{
    MorphMap m; m.start = (Morph*)std::malloc(2 * sizeof(Morph)); m.finish = m.start + 2; m.eos = m.finish; m.Dirty = true;
    m.start[0].key = 5; m.start[1].key = 7;
    Vec v; v.start = 0; v.finish = 0; v.eos = 0;
    v.insert_overflow(v.finish, m, 3, true);           // three deep copies
    if (v.size() != 3 || v.eos - v.start != 3 || !v.start[2].Dirty || v.start[1].start == m.start ||
        v.start[1].start[1].key != 7 || v.start[0].finish - v.start[0].start != 2) return fail();
    MorphMap e;
    v.insert_overflow(v.start + 1, e, 1, false);       // {m, e, m, m}, len 6
    if (v.size() != 4 || v.eos - v.start != 6 || v.start[1].start != 0 || v.start[1].Dirty || v.start[3].start[0].key != 5) return fail();
    v.clear();
    std::free(m.start);
    std::printf("0086e1c0_TEST PASS\n");
    return 0;
}