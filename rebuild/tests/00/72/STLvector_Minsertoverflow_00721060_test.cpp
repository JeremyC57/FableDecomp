// Behaviour test for 0x00721060: vector _M_insert_overflow (grow to old + max(old, n), head, n copies, tail, free old)
#include <cstdio>
#include <cstdlib>

struct Entry { long key; long tex; unsigned long blend[4]; };   // 24 bytes on x86-32

struct Vec {
    Entry* start;
    Entry* finish;
    Entry* end_of_storage;
    void insert_overflow(Entry* pos, const Entry& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = (unsigned int)(finish - start);
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Entry* ns = len != 0 ? (Entry*)std::malloc(len * sizeof(Entry)) : 0;
        Entry* nf = ns;
        for (Entry* p = start; p != pos; ++p, ++nf) *nf = *p;
        for (unsigned int i = 0; i < fill_len; ++i, ++nf) *nf = x;
        if (!atend)
            for (Entry* p = pos; p != finish; ++p, ++nf) *nf = *p;
        if (start != 0) std::free(start);
        start = ns; finish = nf; end_of_storage = ns + len;
    }
};

static Entry Make(long k) { Entry e; e.key = k; e.tex = k * 2; for (int i = 0; i < 4; ++i) e.blend[i] = (unsigned long)(k + i); return e; }

int main()
{
    Vec v; v.start = v.finish = v.end_of_storage = 0;
    v.insert_overflow(v.finish, Make(1), 1, true);                 // empty -> capacity 1
    if (v.finish - v.start != 1 || v.end_of_storage - v.start != 1 || v.start[0].key != 1) { std::printf("00721060_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, Make(2), 1, true);                 // 1 -> 2
    v.insert_overflow(v.start + 1, Make(9), 3, false);             // 2 -> 2 + 3 = 5, middle insert of 3 copies
    if (v.finish - v.start != 5 || v.end_of_storage - v.start != 5) { std::printf("00721060_TEST FAIL\n"); return 1; }
    long want[5] = { 1, 9, 9, 9, 2 };
    for (int i = 0; i < 5; ++i)
        if (v.start[i].key != want[i] || v.start[i].tex != want[i] * 2 || v.start[i].blend[3] != (unsigned long)(want[i] + 3)) { std::printf("00721060_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, Make(4), 1, true);                 // 5 -> 10
    if (v.finish - v.start != 6 || v.end_of_storage - v.start != 10 || v.start[5].key != 4) { std::printf("00721060_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("00721060_TEST PASS\n");
    return 0;
}