#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour test for 00f3b330: vector<CScriptEntry>::_M_insert_overflow -- capacity becomes size + max(size, fillLen),
// prefix / fill / tail (unless atEnd) land in order, the old block is freed.
struct Entry { unsigned int Flags; int BitCount; unsigned int RangeBias; int RangeShift; unsigned int ShuffleMask; unsigned int OrMask; };
static Entry* uninit_copy(Entry* first, Entry* last, Entry* result)
{
    Entry* cur = result;
    for (; first != last; ++first, ++cur) new (cur) Entry(*first);
    return cur;
}
static Entry* uninit_fill_n(Entry* first, unsigned int n, const Entry& x)
{
    Entry* cur = first;
    for (; n > 0; --n, ++cur) new (cur) Entry(x);
    return cur;
}
struct EntryVec {
    Entry* start; Entry* finish; Entry* eos;
    EntryVec() : start(0), finish(0), eos(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Entry* pos, const Entry& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        Entry* ns = len != 0 ? (Entry*)std::malloc(len * sizeof(Entry)) : 0;
        Entry* nf = uninit_copy(start, pos, ns);
        if (fillLen == 1) { new (nf) Entry(x); ++nf; }
        else nf = uninit_fill_n(nf, fillLen, x);
        if (!atEnd) nf = uninit_copy(pos, finish, nf);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static Entry make(unsigned int f) { Entry e; e.Flags = f; e.BitCount = (int)f * 2; e.RangeBias = f + 7; e.RangeShift = 1; e.ShuffleMask = 0; e.OrMask = f; return e; }
int main()
{
    EntryVec v;
    v.insert_overflow(v.finish, make(1), 1, true);
    int ok = v.size() == 1 && v.eos - v.start == 1 && v.start[0].Flags == 1;
    v.insert_overflow(v.finish, make(2), 2, true);           // len = 1 + 2 = 3
    ok = ok && v.size() == 3 && v.eos - v.start == 3 && v.start[1].Flags == 2 && v.start[2].BitCount == 4;
    v.insert_overflow(v.start + 1, make(9), 1, false);       // len = 3 + 3 = 6, middle insert keeps the tail
    ok = ok && v.size() == 4 && v.eos - v.start == 6 && v.start[0].Flags == 1 && v.start[1].Flags == 9
            && v.start[2].Flags == 2 && v.start[3].Flags == 2 && v.start[1].RangeBias == 16;
    std::free(v.start);
    if (!ok) { std::printf("00f3b330_TEST FAIL\n"); return 1; }
    std::printf("00f3b330_TEST PASS\n");
    return 0;
}