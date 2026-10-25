// Behaviour test for 0x00993310 std::vector<pair<long,CSoundBankRecord> >::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CSoundBankRecord {
    int tag;
    CSoundBankRecord(int t) : tag(t) { ++g_live; }
    CSoundBankRecord(const CSoundBankRecord& o) : tag(o.tag) { ++g_live; }
    ~CSoundBankRecord() { --g_live; }
};
struct Entry {
    long first; CSoundBankRecord second;
    Entry(long k, int t) : first(k), second(t) { }
};

struct Vec {
    Entry* start; Entry* finish; Entry* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void InsertOverflow(Entry* pos, const Entry& x, unsigned int fill, bool atend)
    {
        const unsigned int old = size();
        const unsigned int len = old + (old < fill ? fill : old);
        Entry* ns = len != 0 ? (Entry*)std::malloc(len * sizeof(Entry)) : 0;
        Entry* nf = ns;
        for (Entry* p = start; p != pos; ++p, ++nf) new (nf) Entry(*p);
        if (fill == 1) { new (nf) Entry(x); ++nf; }
        else for (unsigned int n = fill; n > 0; --n, ++nf) new (nf) Entry(x);
        if (!atend) for (Entry* p = pos; p != finish; ++p, ++nf) new (nf) Entry(*p);
        for (Entry* p = start; p != finish; ++p) p->~Entry();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    Entry a(1, 10);
    v.InsertOverflow(v.finish, a, 1, true);                 // empty -> 1 (len = 0 + max(0,1))
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].second.tag != 10) { std::printf("00993310_TEST FAIL\n"); return 1; }
    Entry b(2, 20);
    v.InsertOverflow(v.finish, b, 1, true);                 // 1 -> 2
    Entry c(3, 30);
    v.InsertOverflow(v.start + 1, c, 3, false);             // insert 3 copies in the middle: len = 2 + max(2,3) = 5
    const long want[5] = { 1, 3, 3, 3, 2 };
    if (v.size() != 5 || v.eos - v.start != 5) { std::printf("00993310_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v.start[i].first != want[i]) { std::printf("00993310_TEST FAIL\n"); return 1; }
    if (g_live != 3 + 5) { std::printf("00993310_TEST FAIL\n"); return 1; }   // a, b, c locals + 5 elements
    for (Entry* p = v.start; p != v.finish; ++p) p->~Entry();
    std::free(v.start);
    std::printf("00993310_TEST PASS\n");
    return 0;
}