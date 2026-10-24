// Behaviour test for 0x007196D0 vector<CCameraTag>::_M_insert_overflow: grow to old + max(old, fill_len), head, fill, tail.
#include <cstdio>
#include <cstdlib>

struct CCameraTag { int PositionPerson, Position, FocusPerson, FocusPosition, ZoomType, View; };
struct TagVector {
    CCameraTag* start; CCameraTag* finish; CCameraTag* eos;
    void InsertOverflow(CCameraTag* pos, const CCameraTag& x, unsigned int fill, bool atend)
    {
        unsigned int old = (unsigned int)(finish - start);
        unsigned int len = old + (old < fill ? fill : old);
        CCameraTag* ns = len != 0 ? (CCameraTag*)std::malloc(len * sizeof(CCameraTag)) : 0;
        CCameraTag* nf = ns;
        for (CCameraTag* p = start; p != pos; ++p) *nf++ = *p;
        for (unsigned int n = fill; n > 0; --n) *nf++ = x;
        if (!atend) for (CCameraTag* p = pos; p != finish; ++p) *nf++ = *p;
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    TagVector v; v.start = v.finish = v.eos = 0;
    CCameraTag a = { 2, 3, 1, 0, 0, 1 }, b = { 1, 0, 2, 3, 1, 0 }, c = { 9, 9, 9, 9, 9, 9 };
    v.InsertOverflow(v.finish, a, 1, true);
    v.InsertOverflow(v.finish, b, 1, true);
    bool ok = v.finish - v.start == 2 && v.eos - v.start == 2 && v.start[1].FocusPosition == 3;
    v.InsertOverflow(v.start + 1, c, 2, false);
    ok = ok && v.finish - v.start == 4 && v.eos - v.start == 4 && v.start[0].Position == 3 && v.start[1].View == 9
        && v.start[2].View == 9 && v.start[3].FocusPosition == 3;
    std::free(v.start);
    if (!ok) { std::printf("007196d0_TEST FAIL\n"); return 1; }
    std::printf("007196d0_TEST PASS\n");
    return 0;
}