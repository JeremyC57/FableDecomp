// Behaviour test for 0x005149F0 vector<unsigned short>::_M_fill_insert (POD path, memmove).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Vec {
    unsigned short* start; unsigned short* finish; unsigned short* eos;
    void insert_overflow(unsigned short* pos, const unsigned short& x, unsigned int fill_len)
    {
        unsigned int old_size = (unsigned int)(finish - start);
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        unsigned short* ns = (unsigned short*)std::malloc(len * sizeof(unsigned short));
        unsigned short* nf = ns;
        for (unsigned short* p = start; p != pos; ++p) *nf++ = *p;
        for (unsigned int i = 0; i < fill_len; ++i) *nf++ = x;
        for (unsigned short* p = pos; p != finish; ++p) *nf++ = *p;
        std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(unsigned short* pos, unsigned int n, const unsigned short& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            unsigned short x_copy = x;
            unsigned int after = (unsigned int)(finish - pos);
            unsigned short* old_finish = finish;
            if (after > n) {
                if (finish != finish - n) std::memmove(finish, finish - n, n * sizeof(unsigned short));
                finish += n;
                int num = (int)((char*)(old_finish - n) - (char*)pos);
                if (num > 0) std::memmove((char*)old_finish - num, pos, num);
                for (unsigned short* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                unsigned short* o = finish;
                for (unsigned int k = n - after; k > 0; --k) *o++ = x_copy;
                finish += n - after;
                if (old_finish != pos) std::memmove(finish, pos, (char*)old_finish - (char*)pos);
                finish += after;
                for (unsigned short* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            insert_overflow(pos, x, n);
    }
};

static int check(const Vec& v, const unsigned short* w, unsigned int n)
{
    if ((unsigned int)(v.finish - v.start) != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (v.start[i] != w[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    unsigned short a = 1, b = 2, c = 3;
    v.fill_insert(v.finish, 3, a);                        // [1 1 1] cap 3
    v.fill_insert(v.start + 1, 2, b);                     // grow: [1 2 2 1 1] cap 6
    { unsigned short w[] = { 1, 2, 2, 1, 1 }; if (!check(v, w, 5) || v.eos - v.start != 6) ok = 0; }
    v.start[4] = 9;
    v.fill_insert(v.start + 4, 1, c);                     // in place, after (1) == n (1): [1 2 2 1 3 9]
    { unsigned short w[] = { 1, 2, 2, 1, 3, 9 }; if (!check(v, w, 6)) ok = 0; }
    v.fill_insert(v.start + 2, 0, c);                     // no-op
    v.eos = v.start + 6;
    std::free(v.start);
    Vec u; u.start = (unsigned short*)std::malloc(8 * sizeof(unsigned short)); u.finish = u.start; u.eos = u.start + 8;
    for (int i = 0; i < 5; ++i) *u.finish++ = (unsigned short)(10 + i);
    u.fill_insert(u.start + 1, 2, c);                     // in place, after (4) > n (2)
    { unsigned short w[] = { 10, 3, 3, 11, 12, 13, 14 }; if (!check(u, w, 7)) ok = 0; }
    std::free(u.start);
    if (!ok) { std::printf("005149f0_TEST FAIL\n"); return 1; }
    std::printf("005149f0_TEST PASS\n");
    return 0;
}