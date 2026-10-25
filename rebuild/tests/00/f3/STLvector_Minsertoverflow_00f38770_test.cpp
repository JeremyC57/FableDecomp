#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00f38770: vector<CResourcePage>::_M_insert_overflow -- capacity becomes size + max(size, fillLen),
// the prefix, fill and (unless atEnd) tail are deep-copied in order, the old block is destroyed and freed.
struct LongVec {
    long* start; long* finish; long* eos;
    LongVec() : start(0), finish(0), eos(0) { }
    LongVec(const LongVec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (long*)std::malloc(n * sizeof(long)) : 0;
        finish = start;
        eos = start + n;
        if (x.finish != x.start) {
            std::memmove(start, x.start, (x.finish - x.start) * sizeof(long));
            finish = start + (x.finish - x.start);
        }
    }
    ~LongVec() { if (start) std::free(start); }
};
struct Page { long scratchPool; LongVec list; unsigned char* nextFree; long freeSpace; bool autoRestore; };
static Page* uninit_copy(Page* first, Page* last, Page* result)
{
    Page* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Page(*first);
    return cur;
}
static Page* uninit_fill_n(Page* first, unsigned int n, const Page& x)
{
    Page* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) Page(x);
    return cur;
}
struct PageVec {
    Page* start; Page* finish; Page* eos;
    PageVec() : start(0), finish(0), eos(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear_storage()                               // _M_clear 0x00F38710
    {
        for (Page* p = start; p != finish; ++p) p->~Page();
        if (start) std::free(start);
    }
    void insert_overflow(Page* pos, const Page& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        Page* newStart = len != 0 ? (Page*)std::malloc(len * sizeof(Page)) : 0;
        Page* newFinish = uninit_copy(start, pos, newStart);
        if (fillLen == 1) { new (newFinish) Page(x); ++newFinish; }
        else newFinish = uninit_fill_n(newFinish, fillLen, x);
        if (!atEnd) newFinish = uninit_copy(pos, finish, newFinish);
        clear_storage();
        start = newStart; finish = newFinish; eos = newStart + len;
    }
    void push_back(const Page& x)
    {
        if (finish != eos) { new (finish) Page(x); ++finish; }
        else insert_overflow(finish, x, 1, true);
    }
};
static void set_page(Page& p, long pool, long a, long b)
{
    p.scratchPool = pool; p.nextFree = (unsigned char*)0; p.freeSpace = pool * 10; p.autoRestore = (pool & 1) != 0;
    p.list.start = (long*)std::malloc(2 * sizeof(long)); p.list.start[0] = a; p.list.start[1] = b;
    p.list.finish = p.list.start + 2; p.list.eos = p.list.finish;
}
int main()
{
    PageVec v;
    Page x; set_page(x, 1, 11, 12);
    v.insert_overflow(v.finish, x, 1, true);            // empty: len = 0 + max(0, 1) = 1
    int ok = v.size() == 1 && v.eos - v.start == 1 && v.start[0].scratchPool == 1 && v.start[0].list.start[1] == 12;
    ok = ok && v.start[0].list.start != x.list.start;
    Page y; set_page(y, 2, 21, 22);
    v.insert_overflow(v.finish, y, 3, true);            // len = 1 + max(1, 3) = 4, three copies of y appended
    ok = ok && v.size() == 4 && v.eos - v.start == 4 && v.start[0].scratchPool == 1 && v.start[3].scratchPool == 2;
    ok = ok && v.start[1].list.start[0] == 21 && v.start[1].list.start != v.start[2].list.start;
    Page z; set_page(z, 3, 31, 32);
    v.insert_overflow(v.start + 1, z, 1, false);        // insert in the middle: len = 4 + 4 = 8, tail copied after
    ok = ok && v.size() == 5 && v.eos - v.start == 8 && v.start[0].scratchPool == 1 && v.start[1].scratchPool == 3;
    ok = ok && v.start[2].scratchPool == 2 && v.start[4].scratchPool == 2 && v.start[1].autoRestore && !v.start[2].autoRestore;
    v.clear_storage();
    if (!ok) { std::printf("00f38770_TEST FAIL\n"); return 1; }
    std::printf("00f38770_TEST PASS\n");
    return 0;
}