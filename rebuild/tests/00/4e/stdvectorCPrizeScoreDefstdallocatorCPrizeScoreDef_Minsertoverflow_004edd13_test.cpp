// Behaviour test for 0x004edd13 std::vector<CPrizeScoreDef>::_M_insert_overflow(position, x, __false_type, n, atEnd): the
// block grows to size + max(size, n), the prefix [start, position), n copies of x and (unless atEnd) the suffix
// [position, finish) are copy-constructed in order, the old block is cleared and start/finish/end are re-set.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
static int g_clears = 0;

struct ElemModel {                                  // CPrizeScoreDef stand-in (0x30 bytes retail)
    long Key;
    long Payload[11];
    ElemModel() : Key(0) { for (int i = 0; i < 11; ++i) Payload[i] = 0; }
    ElemModel(const ElemModel& o) : Key(o.Key) { ++g_copies; for (int i = 0; i < 11; ++i) Payload[i] = o.Payload[i]; }
};

static void construct(ElemModel* p, const ElemModel& v) { if (p) new (p) ElemModel(v); }

struct VecModel {
    ElemModel* start; ElemModel* finish; ElemModel* end;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear() { ++g_clears; std::free(start); }
    void insert_overflow(ElemModel* position, const ElemModel& x, unsigned int n, bool atEnd)
    {
        const unsigned int oldSize = size();
        const unsigned int length = oldSize + (oldSize < n ? n : oldSize);
        ElemModel* newStart = length != 0 ? (ElemModel*)std::malloc(length * sizeof(ElemModel)) : 0;
        ElemModel* cur = newStart;
        for (ElemModel* p = start; p != position; ++p, ++cur) construct(cur, *p);
        if (n == 1) { construct(cur, x); ++cur; }
        else for (; n > 0; --n, ++cur) construct(cur, x);
        if (!atEnd) for (ElemModel* p = position; p != finish; ++p, ++cur) construct(cur, *p);
        clear();
        start = newStart; finish = cur; end = newStart + length;
    }
};

static int fail() { std::printf("004edd13_TEST FAIL\n"); return 1; }

int main()
{
    VecModel v;
    v.start = (ElemModel*)std::malloc(3 * sizeof(ElemModel));
    for (int i = 0; i < 3; ++i) { new (v.start + i) ElemModel(); v.start[i].Key = 10 + i; v.start[i].Payload[11 - 1] = 100 + i; }
    v.finish = v.start + 3; v.end = v.finish;
    g_copies = 0;
    ElemModel x; x.Key = 77; x.Payload[0] = 5;
    v.insert_overflow(v.start + 1, x, 2, false);           // middle insert of two copies
    if (v.size() != 5 || v.end - v.start != 6 || g_copies != 5 || g_clears != 1) return fail();
    if (v.start[0].Key != 10 || v.start[1].Key != 77 || v.start[2].Key != 77 || v.start[3].Key != 11 || v.start[4].Key != 12) return fail();
    if (v.start[2].Payload[0] != 5 || v.start[4].Payload[11 - 1] != 102) return fail();
    v.end = v.finish;
    v.insert_overflow(v.finish, x, 1, true);               // push_back path: one copy, no suffix
    if (v.size() != 6 || v.end - v.start != 10 || g_copies != 11 || v.start[5].Key != 77) return fail();
    v.end = v.finish;
    v.insert_overflow(v.start, x, 9, false);               // n larger than size: length = size + n
    if (v.size() != 15 || v.end - v.start != 15 || v.start[8].Key != 77 || v.start[9].Key != 10 || v.start[14].Key != 77) return fail();
    std::free(v.start);
    std::printf("004edd13_TEST PASS\n");
    return 0;
}