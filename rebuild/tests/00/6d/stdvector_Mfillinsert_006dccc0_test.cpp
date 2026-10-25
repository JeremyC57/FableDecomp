// Behaviour test for 0x006DCCC0 vector<CAttackHistoryCombo>::_M_fill_insert (model copy of the STLport algorithm).
#include <cstdio>
#include <cstdlib>
#include <new>

struct Elem { long key; float multiplier; };

struct Vec {
    Elem* _M_start; Elem* _M_finish; Elem* _M_end_of_storage;
    int overflow_calls;
    void _M_insert_overflow(Elem* pos, const Elem& x, unsigned int n)
    {
        ++overflow_calls;
        unsigned int old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = old_size + (old_size < n ? n : old_size);
        Elem* ns = (Elem*)std::malloc(len * sizeof(Elem)); Elem* nf = ns;
        for (Elem* p = _M_start; p != pos; ++p) *nf++ = *p;
        for (unsigned int i = 0; i < n; ++i) *nf++ = x;
        for (Elem* p = pos; p != _M_finish; ++p) *nf++ = *p;
        std::free(_M_start); _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    void _M_fill_insert(Elem* position, unsigned int n, const Elem& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                Elem x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                Elem* old_finish = _M_finish;
                if (elems_after > n) {
                    for (Elem *f = _M_finish - n, *d = _M_finish; f != _M_finish; ++f, ++d) new (d) Elem(*f);
                    _M_finish += n;
                    Elem* r = old_finish; Elem* l = old_finish - n;
                    for (long k = (long)(l - position); k > 0; --k) *--r = *--l;
                    for (Elem* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    Elem* d = _M_finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++d) new (d) Elem(x_copy);
                    _M_finish += n - elems_after;
                    d = _M_finish;
                    for (Elem* f = position; f != old_finish; ++f, ++d) new (d) Elem(*f);
                    _M_finish += elems_after;
                    for (Elem* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                _M_insert_overflow(position, x, n);
        }
    }
};

int main()
{
    Vec v; v._M_start = (Elem*)std::malloc(8 * sizeof(Elem)); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 8;
    v.overflow_calls = 0;
    for (long i = 0; i < 4; ++i) { v._M_finish->key = i; ++v._M_finish; }         // 0 1 2 3
    Elem x; x.key = 9;
    v._M_fill_insert(v._M_start + 1, 2, x);                                       // elems_after 3 > 2: 0 9 9 1 2 3
    int ok = v._M_finish - v._M_start == 6 && v._M_start[0].key == 0 && v._M_start[1].key == 9 && v._M_start[2].key == 9
             && v._M_start[3].key == 1 && v._M_start[5].key == 3;
    x.key = 7;
    v._M_fill_insert(v._M_start + 5, 2, x);                                       // elems_after 1 <= 2: 0 9 9 1 2 7 7 3
    ok = ok && v._M_finish - v._M_start == 8 && v._M_start[5].key == 7 && v._M_start[6].key == 7 && v._M_start[7].key == 3;
    v._M_fill_insert(v._M_start, 0, x);
    ok = ok && v.overflow_calls == 0 && v._M_finish - v._M_start == 8;
    v._M_fill_insert(v._M_finish, 1, x);                                          // no room: overflow
    ok = ok && v.overflow_calls == 1 && v._M_finish - v._M_start == 9 && v._M_start[8].key == 7;
    std::free(v._M_start);
    if (!ok) { std::printf("006dccc0_TEST FAIL\n"); return 1; }
    std::printf("006dccc0_TEST PASS\n");
    return 0;
}