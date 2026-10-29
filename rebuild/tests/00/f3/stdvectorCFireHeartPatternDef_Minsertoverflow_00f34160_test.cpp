// Behaviour test for 0x00F34160 std::vector<CFireHeartPatternDef>::_M_insert_overflow: grow to max(2 * size, size + n),
// copy-construct the prefix, n copies of x (one by placement copy when n == 1), and the suffix unless atend, destroy and free
// the old storage, and adopt the new block.
#include <stdio.h>
#include <stdlib.h>
#include <new>

static int g_live, g_copies;
struct CDef {
    int Value;
    CDef(int v) : Value(v) { ++g_live; }
    CDef(const CDef& o) : Value(o.Value) { ++g_live; ++g_copies; }
    ~CDef() { --g_live; }
};
struct CVec {
    CDef* Start; CDef* Finish; CDef* End;
    int size() const { return (int)(Finish - Start); }
    void InsertOverflow(CDef* pos, const CDef& x, int n, bool atend)
    {
        int oldSize = size();
        int len = oldSize + (oldSize > n ? oldSize : n);
        CDef* newStart = (CDef*)malloc(len * sizeof(CDef));
        CDef* newFinish = newStart;
        for (CDef* p = Start; p != pos; ++p, ++newFinish) new ((void*)newFinish) CDef(*p);
        if (n == 1) { new ((void*)newFinish) CDef(x); ++newFinish; }
        else for (int i = 0; i < n; ++i, ++newFinish) new ((void*)newFinish) CDef(x);
        if (!atend)
            for (CDef* p = pos; p != Finish; ++p, ++newFinish) new ((void*)newFinish) CDef(*p);
        for (CDef* p = Start; p != Finish; ++p) p->~CDef();
        free(Start);
        Start = newStart; Finish = newFinish; End = newStart + len;
    }
};

int main()
{
    CVec v; v.Start = (CDef*)malloc(3 * sizeof(CDef));
    for (int i = 0; i < 3; ++i) new ((void*)(v.Start + i)) CDef(i + 1);
    v.Finish = v.Start + 3; v.End = v.Finish;
    CDef x(9);
    v.InsertOverflow(v.Start + 1, x, 2, false);
    int ok = v.size() == 5 && v.End - v.Start == 6 && v.Start[0].Value == 1 && v.Start[1].Value == 9 && v.Start[2].Value == 9
        && v.Start[3].Value == 2 && v.Start[4].Value == 3 && g_live == 6;
    v.InsertOverflow(v.Finish, x, 1, true);
    ok = ok && v.size() == 6 && v.End - v.Start == 10 && v.Start[5].Value == 9 && g_live == 7;
    v.InsertOverflow(v.Start, x, 20, false);
    ok = ok && v.size() == 26 && v.End - v.Start == 26 && v.Start[19].Value == 9 && v.Start[20].Value == 1 && g_live == 27;
    if (!ok) { printf("00f34160_TEST FAIL\n"); return 1; }
    printf("00f34160_TEST PASS\n");
    return 0;
}