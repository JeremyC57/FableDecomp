// behaviour test for 0x00472346 std::vector<EOpinionAttitudeType >::_M_insert_overflow: the new block holds size + max(size, fill_len)
// elements; head, fill_len copies of x and (unless atend) the tail land in order; the old block is freed
#include <cstdio>
#include <cstdlib>
struct Vec { int* start; int* finish; int* eos; };
static int gAllocs = 0, gFrees = 0;
static void InsertOverflow(Vec& v, int* pos, const int& x, unsigned int fillLen, bool atend)
{
    unsigned int oldSize = (unsigned int)(v.finish - v.start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    int* ns = len ? (++gAllocs, (int*)std::malloc(len * sizeof(int))) : 0;
    int* nf = ns;
    for (int* p = v.start; p != pos; ++p, ++nf) *nf = *p;
    if (fillLen == 1) { *nf = x; ++nf; }
    else for (unsigned int n = fillLen; n > 0; --n, ++nf) *nf = x;
    if (!atend) for (int* p = pos; p != v.finish; ++p, ++nf) *nf = *p;
    if (v.start) { ++gFrees; std::free(v.start); }
    v.start = ns; v.finish = nf; v.eos = ns + len;
}
int main()
{
    int fails = 0;
    Vec v; v.start = v.finish = v.eos = 0;
    InsertOverflow(v, v.finish, 7, 1, true);
    if (gAllocs != 1 || v.finish - v.start != 1 || v.eos != v.finish || v.start[0] != 7) fails |= 1;
    InsertOverflow(v, v.start, 3, 3, false);
    if (gAllocs != 2 || gFrees != 1 || v.finish - v.start != 4 || v.eos - v.start != 4) fails |= 2;
    if (v.start[0] != 3 || v.start[2] != 3 || v.start[3] != 7) fails |= 4;
    InsertOverflow(v, v.start + 1, 9, 1, false);
    if (v.finish - v.start != 5 || v.eos - v.start != 8 || v.start[1] != 9 || v.start[4] != 7) fails |= 8;
    std::free(v.start);
    if (fails) { std::printf("00472346_TEST FAIL %d\n", fails); return 1; }
    std::printf("00472346_TEST PASS\n");
    return 0;
}