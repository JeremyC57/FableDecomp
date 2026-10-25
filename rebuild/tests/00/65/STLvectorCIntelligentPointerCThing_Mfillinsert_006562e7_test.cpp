// Behaviour test for 0x006562E7: STLport vector::_M_fill_insert (in-place shift for both elems_after cases, overflow path)
#include <cstdio>
#include <new>

struct Ptr {
    int item;
    Ptr() : item(0) { }
    Ptr(int i) : item(i) { }
    Ptr(const Ptr& o) : item(o.item) { }
    Ptr& operator=(const Ptr& o) { item = o.item; return *this; }
};

struct Vec {
    Ptr* start;
    Ptr* finish;
    Ptr* eos;
    int overflow;
    void insert_overflow(Ptr* pos, const Ptr& x, unsigned int n)
    {
        ++overflow;
        static unsigned char raw[64 * sizeof(Ptr)];
        Ptr* s = (Ptr*)raw;
        Ptr* f = s;
        for (Ptr* p = start; p != pos; ++p, ++f) new (f) Ptr(*p);
        for (unsigned int k = 0; k < n; ++k, ++f) new (f) Ptr(x);
        for (Ptr* p = pos; p != finish; ++p, ++f) new (f) Ptr(*p);
        start = s; finish = f; eos = s + 64;
    }
    void fill_insert(Ptr* position, unsigned int n, const Ptr& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                Ptr x_copy = x;
                const unsigned int elems_after = (unsigned int)(finish - position);
                Ptr* old_finish = finish;
                if (elems_after > n) {
                    Ptr* d = finish;
                    for (Ptr* s = finish - n; s != finish; ++s, ++d) new (d) Ptr(*s);
                    finish += n;
                    Ptr* r = old_finish; Ptr* l = old_finish - n;
                    for (int k = (int)(l - position); k > 0; --k) *--r = *--l;
                    for (Ptr* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    Ptr* d = finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++d) new (d) Ptr(x_copy);
                    finish += n - elems_after;
                    d = finish;
                    for (Ptr* s = position; s != old_finish; ++s, ++d) new (d) Ptr(*s);
                    finish += elems_after;
                    for (Ptr* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                insert_overflow(position, x, n);
        }
    }
};

static bool Check(const Vec& v, const int* want, int n)
{
    if (v.finish - v.start != n) return false;
    for (int i = 0; i < n; ++i) if (v.start[i].item != want[i]) return false;
    return true;
}

int main()
{
    static unsigned char raw[16 * sizeof(Ptr)];
    Vec v; v.start = (Ptr*)raw; v.finish = v.start; v.eos = v.start + 16; v.overflow = 0;
    for (int i = 1; i <= 5; ++i) { new (v.finish) Ptr(i); ++v.finish; }
    v.fill_insert(v.start + 1, 2, Ptr(9));          // elems_after 4 > 2
    int w1[7] = { 1, 9, 9, 2, 3, 4, 5 };
    if (!Check(v, w1, 7)) { std::printf("006562e7_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start + 5, 3, Ptr(7));          // elems_after 2 <= 3
    int w2[10] = { 1, 9, 9, 2, 3, 7, 7, 7, 4, 5 };
    if (!Check(v, w2, 10)) { std::printf("006562e7_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start, 0, Ptr(1));
    if (!Check(v, w2, 10) || v.overflow != 0) { std::printf("006562e7_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start + 2, 8, Ptr(6));          // 6 free < 8: overflow
    if (v.overflow != 1 || v.finish - v.start != 18 || v.start[2].item != 6 || v.start[9].item != 6 || v.start[10].item != 9) {
        std::printf("006562e7_TEST FAIL\n"); return 1;
    }
    std::printf("006562e7_TEST PASS\n");
    return 0;
}