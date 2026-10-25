// Behaviour test for 0x005E29D0 vector<pair<EAnimComponent, CIVCountedPointer<..>>>::_M_insert_overflow: the new block holds max(2*size, size+fill_len) slots; head, fill_len add-ref'd copies of x and (unless atend) the tail are copy-constructed; the old elements are released and the old block freed.
#include <cstdio>
#include <cstdlib>
#include <new>
struct Obj { long refs; };
struct Ptr {
    Obj* o;
    Ptr(Obj* p) : o(p) { if (o) ++o->refs; }
    Ptr(const Ptr& x) : o(x.o) { if (o) ++o->refs; }
    ~Ptr() { if (o) { --o->refs; o = 0; } }
};
struct Pair { int first; Ptr second; Pair(int k, Obj* p) : first(k), second(p) { } };
struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Pair* pos, const Pair& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Pair* ns = len ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ns;
        for (Pair* p = start; p != pos; ++p, ++nf) new (nf) Pair(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++nf) new (nf) Pair(x);
        if (!atend) for (Pair* p = pos; p != finish; ++p, ++nf) new (nf) Pair(*p);
        for (Pair* p = start; p != finish; ++p) p->~Pair();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static int fail() { std::printf("005e29d0_TEST FAIL\n"); return 1; }
int main()
{
    Obj a = { 0 }, b = { 0 }, x = { 0 };
    Vec v; v.start = (Pair*)std::malloc(2 * sizeof(Pair)); v.finish = v.start; v.eos = v.start + 2;
    new (v.finish++) Pair(1, &a);
    new (v.finish++) Pair(2, &b);
    Pair val(9, &x);
    v.insert_overflow(v.start + 1, val, 1, false);     // middle insert: {1, 9, 2}, capacity 4
    if (v.size() != 3 || v.eos - v.start != 4 || v.start[0].first != 1 || v.start[1].first != 9 || v.start[2].first != 2 ||
        a.refs != 1 || b.refs != 1 || x.refs != 2) return fail();
    v.insert_overflow(v.finish, val, 5, true);         // fill at the end: len = 3 + max(3, 5) = 8
    if (v.size() != 8 || v.eos - v.start != 8 || x.refs != 7 || v.start[7].first != 9 || a.refs != 1) return fail();
    for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    std::free(v.start);
    if (a.refs != 0 || b.refs != 0 || x.refs != 1) return fail();
    std::printf("005e29d0_TEST PASS\n");
    return 0;
}