// Behaviour test for vector<CRegion>::_M_insert_overflow (0x0051CF10): growth to old + max(old, n), head copy,
// single or n-fold insert, optional tail copy, old block destroyed -- through a copy of the model.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0, g_dtors = 0;

struct Region {                         // stands in for the 0x58-byte CRegion
    int id;
    Region(int i) : id(i) {}
    Region(const Region& o) : id(o.id) { ++g_copies; }
    ~Region() { ++g_dtors; }
};

static Region* UninitCopy(Region* first, Region* last, Region* result)
{
    for (; first != last; ++first, ++result) new (result) Region(*first);
    return result;
}
static Region* UninitFillN(Region* first, unsigned int n, const Region& x)
{
    for (; n > 0; --n, ++first) new (first) Region(x);
    return first;
}

struct RegionVector {
    Region* start; Region* finish; Region* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void InsertOverflow(Region* position, const Region& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Region* new_start = len != 0 ? (Region*)std::malloc(len * sizeof(Region)) : 0;
        Region* new_finish = UninitCopy(start, position, new_start);
        if (fill_len == 1) { new (new_finish) Region(x); ++new_finish; }
        else new_finish = UninitFillN(new_finish, fill_len, x);
        if (!atend) new_finish = UninitCopy(position, finish, new_finish);
        for (Region* p = start; p != finish; ++p) p->~Region();
        if (start) std::free(start);
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};

int main()
{
    RegionVector v;
    v.start = (Region*)std::malloc(4 * sizeof(Region));
    for (int i = 0; i < 4; ++i) new (v.start + i) Region(i);
    v.finish = v.eos = v.start + 4;

    Region x(99);
    g_copies = g_dtors = 0;
    v.InsertOverflow(v.start + 1, x, 1, false);          // insert one in the middle: capacity 4 + 4
    if (v.size() != 5 || v.eos - v.start != 8 || g_copies != 5 || g_dtors != 4) { std::printf("0051cf10_TEST FAIL\n"); return 1; }
    if (v.start[0].id != 0 || v.start[1].id != 99 || v.start[2].id != 1 || v.start[4].id != 3) { std::printf("0051cf10_TEST FAIL\n"); return 1; }

    g_copies = g_dtors = 0;
    v.InsertOverflow(v.finish, x, 7, true);              // push 7 at the end: len = 5 + max(5, 7) = 12, tail skipped
    if (v.size() != 12 || v.eos - v.start != 12 || g_copies != 12 || g_dtors != 5) { std::printf("0051cf10_TEST FAIL\n"); return 1; }
    for (int i = 5; i < 12; ++i) if (v.start[i].id != 99) { std::printf("0051cf10_TEST FAIL\n"); return 1; }

    RegionVector e; e.start = e.finish = e.eos = 0;     // empty vector: len = max(0, 1) = 1
    e.InsertOverflow(e.finish, x, 1, true);
    if (e.size() != 1 || e.start[0].id != 99 || e.eos - e.start != 1) { std::printf("0051cf10_TEST FAIL\n"); return 1; }
    std::printf("0051cf10_TEST PASS\n");
    return 0;
}