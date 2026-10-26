// behaviour test for 0x009C2CA0 vector<ESurfaceMultisampleType >::_M_insert_overflow: the new block holds size + max(size, fill_len) slots, the head [start, position) is copied, x lands once (or fill_len times), the tail [position, finish) follows unless atend, and the vector adopts the new block
#include <cstdio>
#include <cstdlib>
struct Vec { long* start; long* finish; long* eos; };
static void InsertOverflow(Vec& v, long* position, const long& x, unsigned int fill_len, bool atend)   // the function under test
{
    unsigned int old_size = (unsigned int)(v.finish - v.start);
    unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
    long* ns = len ? (long*)std::malloc(len * sizeof(long)) : 0;
    long* nf = ns;
    for (long* p = v.start; p != position; ++p, ++nf) *nf = *p;
    if (fill_len == 1) { *nf = x; ++nf; }
    else for (unsigned int n = fill_len; n > 0; --n, ++nf) *nf = x;
    if (!atend) for (long* p = position; p != v.finish; ++p, ++nf) *nf = *p;
    if (v.start) std::free(v.start);
    v.start = ns; v.finish = nf; v.eos = ns + len;
}
int main()
{
    int fail = 0;
    Vec v; v.start = (long*)std::malloc(4 * sizeof(long)); v.finish = v.start + 4; v.eos = v.finish;
    for (int i = 0; i < 4; ++i) v.start[i] = 10 + i;
    InsertOverflow(v, v.start + 1, 99, 3, false);                     // middle insert of three copies
    long want[7] = { 10, 99, 99, 99, 11, 12, 13 };
    if (v.finish - v.start != 7 || v.eos - v.start != 8) fail |= 1;   // 4 + max(4, 3)
    for (int i = 0; i < 7; ++i) if (v.start[i] != want[i]) fail |= 2;
    long seven = 7;
    InsertOverflow(v, v.finish, seven, 1, true);                       // push_back growth
    if (v.finish - v.start != 8 || v.eos - v.start != 14 || v.start[7] != 7 || v.start[6] != 13) fail |= 4;
    std::free(v.start);
    if (fail) { std::printf("009c2ca0_TEST FAIL %d\n", fail); return 1; }
    std::printf("009c2ca0_TEST PASS\n");
    return 0;
}