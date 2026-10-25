// Behaviour test for 0x00759290 std::vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_fill_insert: n == 0 is a
// no-op; with spare capacity both tail cases (elems_after > n: copy the last n up, copy_backward, fill; elems_after <=
// n: fill_n past the end, copy the tail up, fill the gap); without capacity the _M_insert_overflow growth
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

struct Elem {                                         // 44 bytes: location key + the 40-byte wound entry
    int first;
    int Location;
    int Indices[8];
    float PermanentScarStrength;
};

struct Vec {
    Elem* s;
    Elem* f;
    Elem* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    void insert_overflow(Elem* pos, const Elem& x, unsigned int n)
    {
        unsigned int old = size();
        unsigned int len = old + (old < n ? n : old);
        Elem* ns = (Elem*)std::malloc(len * sizeof(Elem));
        Elem* nf = ns;
        for (Elem* p = s; p != pos; ++p) *nf++ = *p;
        for (unsigned int i = 0; i < n; ++i) *nf++ = x;
        for (Elem* p = pos; p != f; ++p) *nf++ = *p;
        std::free(s);
        s = ns; f = nf; e = ns + len;
    }
    void fill_insert(Elem* pos, unsigned int n, const Elem& x)
    {
        if (n != 0) {
            if ((unsigned int)(e - f) >= n) {
                Elem x_copy = x;                      // x may live inside the vector
                const unsigned int elems_after = (unsigned int)(f - pos);
                Elem* old_finish = f;
                if (elems_after > n) {
                    for (unsigned int i = 0; i < n; ++i) f[i] = *(f - n + i);
                    f += n;
                    for (Elem* p = old_finish - n; p != pos; ) { --p; *(p + n) = *p; }
                    for (unsigned int i = 0; i < n; ++i) pos[i] = x_copy;
                }
                else {
                    for (unsigned int i = 0; i < n - elems_after; ++i) f[i] = x_copy;
                    f += n - elems_after;
                    for (Elem* p = pos; p != old_finish; ++p) *f++ = *p;
                    for (Elem* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            }
            else
                insert_overflow(pos, x, n);
        }
    }
};

Elem make(int k)
{
    Elem x;
    x.first = k; x.Location = k;
    for (int i = 0; i < 8; ++i) x.Indices[i] = k * 10 + i;
    x.PermanentScarStrength = (float)k;
    return x;
}
bool keys(const Vec& v, const int* want, unsigned int n)
{
    if (v.size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (v.s[i].first != want[i] || v.s[i].Indices[7] != want[i] * 10 + 7 || v.s[i].PermanentScarStrength != (float)want[i]) return false;
    return true;
}

}

int main()
{
    using namespace model;
    int bad = 0;
    Vec v;
    v.s = (Elem*)std::malloc(10 * sizeof(Elem));
    v.f = v.s;
    v.e = v.s + 10;
    for (int i = 1; i <= 4; ++i) *v.f++ = make(i);
    v.fill_insert(v.s + 1, 0, make(9));                                       // no-op
    int w0[4] = { 1, 2, 3, 4 };
    if (!keys(v, w0, 4)) bad = 1;
    v.fill_insert(v.s + 1, 2, make(7));                                       // elems_after 3 > 2
    int w1[6] = { 1, 7, 7, 2, 3, 4 };
    if (!keys(v, w1, 6) || v.capacity() != 10) bad = 2;
    v.fill_insert(v.s + 5, 3, make(8));                                       // elems_after 1 <= 3
    int w2[9] = { 1, 7, 7, 2, 3, 8, 8, 8, 4 };
    if (!keys(v, w2, 9) || v.capacity() != 10) bad = 3;
    v.fill_insert(v.s + 1, v.s[0].first + 1, v.s[0]);                         // x inside the vector, 2 copies: grow 9 + 9
    int w3[11] = { 1, 1, 1, 7, 7, 2, 3, 8, 8, 8, 4 };
    if (!keys(v, w3, 11) || v.capacity() != 18) bad = 4;
    std::free(v.s);
    if (bad) { std::printf("00759290_TEST FAIL\n"); return 1; }
    std::printf("00759290_TEST PASS\n");
    return 0;
}