// Behaviour test for 0x0046cb84: std::vector<EActionRegisteredType>::resize(n, x) -- shrinking erases the tail, growing appends n - size()
// copies of x (STLport: erase(begin() + n, end()) / insert(end(), n - size(), x)).
#include <stdio.h>
struct E { int v[1]; };
struct V {
    E buf[16]; E* first; E* last;
    V() : first(buf), last(buf) { }
    unsigned int size() const { return (unsigned int)(last - first); }
    void erase(E* f, E* l) { E* d = f; for (E* s = l; s != last; ++s, ++d) *d = *s; last = d; }
    void fill_insert(E* pos, unsigned int n, const E& x) { for (unsigned int i = 0; i < n; ++i) *last++ = x; (void)pos; }
    void resize(unsigned int n, const E& x) { if (n < size()) erase(first + n, last); else fill_insert(last, n - size(), x); }
};
static E mk(int k) { E e; for (int i = 0; i < 1; ++i) e.v[i] = k * 100 + i; return e; }
int main()
{
    V v; E a = mk(1), b = mk(2);
    v.resize(3, a);                       // grow from empty
    bool ok = v.size() == 3 && v.first[2].v[0] == 100;
    v.resize(5, b);                       // grow: two copies of b appended
    ok = ok && v.size() == 5 && v.first[2].v[0] == 100 && v.first[3].v[0] == 200 && v.first[4].v[0] == 200 + 0;
    v.resize(2, b);                       // shrink: tail erased, x unused
    ok = ok && v.size() == 2 && v.first[1].v[0] == 100;
    v.resize(2, b);                       // same size: insert of 0
    ok = ok && v.size() == 2 && sizeof(E) == 4;
    if (!ok) { printf("0046cb84_TEST FAIL\n"); return 1; }
    printf("0046cb84_TEST PASS\n");
    return 0;
}
