// Behaviour test for 0x00A73530 std::vector::push_back: room -> copy-construct at the finish and advance it (the counted
// pointer's reference count goes up); full -> the overflow path gets (finish, x, tag, 1, true).
#include <cstdio>
#include <new>

struct SCount { long refs; };
struct SRec {
    long key; const char* name; void* object; SCount* count;
    SRec() : key(0), name(0), object(0), count(0) { }
    SRec(const SRec& o) : key(o.key), name(o.name), object(o.object), count(o.count) { if (count) ++count->refs; }
};
struct SVec {
    SRec* start; SRec* finish; SRec* end;
    int overflowCalls; SRec* overflowPos; unsigned int overflowFill; bool overflowAtEnd;
    void InsertOverflow(SRec* pos, const SRec&, unsigned int fill, bool atend)
    { ++overflowCalls; overflowPos = pos; overflowFill = fill; overflowAtEnd = atend; }
    void push_back(const SRec& x)
    {
        if (finish != end) {
            if (finish) new (finish) SRec(x);
            ++finish;
        }
        else
            InsertOverflow(finish, x, 1, true);
    }
};


int main()
{
    SRec storage[2];
    SVec v; v.start = storage; v.finish = storage; v.end = storage + 2;
    v.overflowCalls = 0; v.overflowPos = 0; v.overflowFill = 0; v.overflowAtEnd = false;
    SCount c; c.refs = 1;
    SRec r; r.key = 7; r.name = "n"; r.object = &c; r.count = &c;
    v.push_back(r);
    v.push_back(r);
    if (v.finish != storage + 2 || storage[1].key != 7 || storage[0].count != &c || c.refs != 3 || v.overflowCalls != 0) {
        std::printf("00a73530_TEST FAIL\n"); return 1;
    }
    v.push_back(r);
    if (v.overflowCalls != 1 || v.overflowPos != storage + 2 || v.overflowFill != 1 || !v.overflowAtEnd || c.refs != 3) {
        std::printf("00a73530_TEST FAIL\n"); return 1;
    }
    std::printf("00a73530_TEST PASS\n");
    return 0;
}