// Behaviour test for 0x007B5030 __copy(deque_iterator, deque_iterator, back_insert_iterator<vector<T> >): the deque
// distance formula and the node-walking increment, n push_backs into the vector.
#include <cstdio>
struct Rec { int a, b; };
struct Vec { Rec items[64]; int n; void push_back(const Rec& r) { items[n++] = r; } };
struct Inserter { Vec* c; Inserter& operator=(const Rec& v) { c->push_back(v); return *this; } Inserter& operator*() { return *this; } Inserter& operator++() { return *this; } };
struct It {
    Rec* cur; Rec* first; Rec* last; Rec** node;
    int operator-(const It& x) const { return 16 * (int)(node - x.node - 1) + (int)(cur - first) + (int)(x.last - x.cur); }
    It& operator++() { if (++cur == last) { node = node + 1; first = *node; last = first + 16; cur = first; } return *this; }
    Rec& operator*() const { return *cur; }
};
static Inserter Copy(It first, It last, Inserter result)
{
    for (int n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Rec b0[16], b1[16], b2[16];
    Rec* map[3] = { b0, b1, b2 };
    int k = 0;
    for (int i = 0; i < 16; ++i) { b0[i].a = k++; b1[i].a = k++ ; b2[i].a = k++; }
    // first = b0[14], last = b2[3]: 2 + 16 + 3 = 21 elements
    It f; f.node = &map[0]; f.first = b0; f.last = b0 + 16; f.cur = b0 + 14;
    It l; l.node = &map[2]; l.first = b2; l.last = b2 + 16; l.cur = b2 + 3;
    if (l - f != 21) { std::printf("007b5030_TEST FAIL\n"); return 1; }
    Vec v; v.n = 0; Inserter ins; ins.c = &v;
    Inserter out = Copy(f, l, ins);
    if (out.c != &v || v.n != 21 || v.items[0].a != b0[14].a || v.items[2].a != b1[0].a || v.items[20].a != b2[2].a) { std::printf("007b5030_TEST FAIL\n"); return 1; }
    std::printf("007b5030_TEST PASS\n");
    return 0;
}