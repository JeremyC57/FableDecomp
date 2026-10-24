// Behaviour test for 0x00710980 vector<short>::_M_insert_overflow: grow to old + max(old, fill_len), head copied, fill_len
// copies of x, tail copied unless appending, old block freed.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct ShortVector {
    short* start; short* finish; short* eos;
    void InsertOverflow(short* pos, const short& x, unsigned int fill, bool atend)
    {
        unsigned int old = (unsigned int)(finish - start);
        unsigned int len = old + (old < fill ? fill : old);
        short* ns = len != 0 ? (short*)std::malloc(len * sizeof(short)) : 0;
        short* nf = ns;
        if (pos != start) { std::memmove(nf, start, (pos - start) * sizeof(short)); nf += pos - start; }
        for (unsigned int n = fill; n > 0; --n) *nf++ = x;
        if (!atend && finish != pos) { std::memmove(nf, pos, (finish - pos) * sizeof(short)); nf += finish - pos; }
        if (start != 0) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    ShortVector v; v.start = v.finish = v.eos = 0;
    short a = 5;
    v.InsertOverflow(v.finish, a, 1, true);                       // empty: len = 0 + max(0, 1) = 1
    bool ok = v.finish - v.start == 1 && v.eos - v.start == 1 && v.start[0] == 5;
    short b = 7;
    v.InsertOverflow(v.finish, b, 1, true);                       // len 2
    short c = 9;
    v.InsertOverflow(v.start + 1, c, 3, false);                   // insert 3 nines in the middle: len = 2 + 3 = 5
    short expect[5] = { 5, 9, 9, 9, 7 };
    ok = ok && v.finish - v.start == 5 && v.eos - v.start == 5;
    for (int i = 0; i < 5; ++i) ok = ok && v.start[i] == expect[i];
    std::free(v.start);
    if (!ok) { std::printf("00710980_TEST FAIL\n"); return 1; }
    std::printf("00710980_TEST PASS\n");
    return 0;
}