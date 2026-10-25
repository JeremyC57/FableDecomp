// Behaviour test for 0x0081D0B0 vector<CTextureMorphEntry>::_M_fill_insert (STLport _M_fill_insert, 20-byte element).
#include <cstdio>
#include <cstdlib>
struct CElem {
    int Tag;
    unsigned char _rest[16];
};
struct CVec {
    CElem* start; CElem* finish; CElem* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CElem* pos, const CElem& x, unsigned int fill)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        CElem* ns = len ? (CElem*)std::malloc(len * sizeof(CElem)) : 0;
        CElem* nf = ns;
        for (CElem* p = start; p != pos; ++p, ++nf) *nf = *p;
        for (unsigned int n = fill; n > 0; --n, ++nf) *nf = x;
        for (CElem* p = pos; p != finish; ++p, ++nf) *nf = *p;
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CElem* pos, unsigned int n, const CElem& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            CElem x_copy = x;
            unsigned int after = (unsigned int)(finish - pos);
            CElem* old_finish = finish;
            if (after > n) {
                for (CElem *s = finish - n, *d = finish; s != old_finish; ++s, ++d) *d = *s;
                finish += n;
                for (CElem *s = old_finish - n, *d = old_finish; s != pos; ) *--d = *--s;
                for (CElem* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                for (unsigned int k = n - after; k > 0; --k) *finish++ = x_copy;
                for (CElem* s = pos; s != old_finish; ++s) *finish++ = *s;
                for (CElem* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            insert_overflow(pos, x, n);
    }
};
static CElem mk(int t) { CElem e; e.Tag = t; return e; }
int main()
{
    int bad = 0;
    if (sizeof(CElem) != 20) bad = 1;
    CVec v; v.start = (CElem*)std::malloc(10 * sizeof(CElem)); v.finish = v.start; v.eos = v.start + 10;
    for (int i = 0; i < 5; ++i) *v.finish++ = mk(i);              // 0 1 2 3 4
    v.fill_insert(v.start + 1, 2, mk(9));                          // after = 4 > 2: 0 9 9 1 2 3 4
    int w1[] = {0, 9, 9, 1, 2, 3, 4};
    if (v.size() != 7) bad = 1;
    for (int i = 0; i < 7 && !bad; ++i) if (v.start[i].Tag != w1[i]) bad = 1;
    v.fill_insert(v.start + 6, 3, mk(7));                          // after = 1 <= 3: 0 9 9 1 2 3 7 7 7 4
    int w2[] = {0, 9, 9, 1, 2, 3, 7, 7, 7, 4};
    if (v.size() != 10) bad = 1;
    for (int i = 0; i < 10 && !bad; ++i) if (v.start[i].Tag != w2[i]) bad = 1;
    v.fill_insert(v.start, 2, mk(5));                              // no room: overflow, capacity 20
    if (v.size() != 12 || v.eos - v.start != 20 || v.start[0].Tag != 5 || v.start[2].Tag != 0 || v.start[11].Tag != 4) bad = 1;
    v.fill_insert(v.start, 0, mk(1));
    if (v.size() != 12) bad = 1;
    std::free(v.start);
    if (bad) { std::printf("0081d0b0_TEST FAIL\n"); return 1; }
    std::printf("0081d0b0_TEST PASS\n");
    return 0;
}