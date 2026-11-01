// Behaviour test for std::vector<CTVertexWaterBackground>::_M_fill_insert 0x00B7FDE0 (standalone copy of the STLport logic).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct V { short X; short Y; float Z; float S[12]; };
struct Vec {
    V* start; V* finish; V* eos;
    void overflow(V* pos, const V& x, unsigned int n)
    {
        unsigned int old = finish - start, len = old + (old > n ? old : n);
        V* ns = (V*)std::malloc(len * sizeof(V)); V* nf = ns;
        for (V* p = start; p != pos; ++p) *nf++ = *p;
        for (unsigned int i = 0; i < n; ++i) *nf++ = x;
        for (V* p = pos; p != finish; ++p) *nf++ = *p;
        std::free(start); start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(V* position, unsigned int n, const V& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                V x_copy = x;
                const unsigned int elems_after = finish - position;
                V* old_finish = finish;
                if (elems_after > n) {
                    V* cur = finish;
                    for (V* f = finish - n; f != finish; ++f, ++cur) *cur = *f;
                    finish += n;
                    V* last = old_finish - n; V* result = old_finish;
                    for (int k = int(last - position); k > 0; --k) *--result = *--last;
                    for (V* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    V* cur = finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++cur) *cur = x_copy;
                    finish += n - elems_after;
                    cur = finish;
                    for (V* f = position; f != old_finish; ++f, ++cur) *cur = *f;
                    finish += elems_after;
                    for (V* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                overflow(position, x, n);
        }
    }
};
static V mk(short x) { V v; std::memset(&v, 0, sizeof v); v.X = x; v.Z = (float)x; return v; }
static bool check(const Vec& v, const char* expect)
{
    unsigned int n = std::strlen(expect);
    if ((unsigned int)(v.finish - v.start) != n) return false;
    for (unsigned int i = 0; i < n; ++i) if (v.start[i].X != expect[i] || v.start[i].Z != (float)expect[i]) return false;
    return true;
}
int main()
{
    Vec v; v.start = (V*)std::malloc(10 * sizeof(V)); v.finish = v.start; v.eos = v.start + 10;
    for (char c = 'a'; c <= 'e'; ++c) *v.finish++ = mk(c);
    v.fill_insert(v.start + 1, 2, mk('X'));           // elems_after 4 > 2
    bool ok = check(v, "aXXbcde");
    v.fill_insert(v.start + 6, 2, mk('Y'));           // elems_after 1 <= 2
    ok = ok && check(v, "aXXbcdYYe");
    v.fill_insert(v.start, 0, mk('Z'));
    ok = ok && check(v, "aXXbcdYYe");
    v.fill_insert(v.start + 9, 3, mk('Q'));           // overflow
    ok = ok && check(v, "aXXbcdYYeQQQ") && (v.eos - v.start) == 18;
    std::free(v.start);
    if (!ok) { std::printf("00b7fde0_TEST FAIL\n"); return 1; }
    std::printf("00b7fde0_TEST PASS\n");
    return 0;
}