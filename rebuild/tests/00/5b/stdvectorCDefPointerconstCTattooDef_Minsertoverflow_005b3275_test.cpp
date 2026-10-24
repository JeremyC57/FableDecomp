// Behaviour test for 0x005B3275 vector<CDefPointer<const CTattooDef> >::_M_insert_overflow (model copy: new storage of old_size + max(old_size, fill_len), head copied, fill_len add-ref'd copies of x, tail copied unless atend, old copies released)
#include <cstdio>
#include <cstdlib>
#include <new>
struct Def { long refs; };
struct Ptr {
    Def* p;
    Ptr(const Ptr& o) : p(o.p) { if (p) ++p->refs; }
    explicit Ptr(Def* d) : p(d) { if (p) ++p->refs; }
    ~Ptr() { if (p) --p->refs; }
};
struct Vec {
    Ptr* start; Ptr* finish; Ptr* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear_() { for (Ptr* q = start; q != finish; ++q) q->~Ptr(); std::free(start); }
    void insert_overflow(Ptr* pos, const Ptr& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Ptr* ns = len ? (Ptr*)std::malloc(len * sizeof(Ptr)) : 0;
        Ptr* nf = ns;
        for (Ptr* q = start; q != pos; ++q, ++nf) new (nf) Ptr(*q);
        if (fill_len == 1) { new (nf) Ptr(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) Ptr(x);
        if (!atend) for (Ptr* q = pos; q != finish; ++q, ++nf) new (nf) Ptr(*q);
        clear_();
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    Def a = { 0 }, b = { 0 }, c = { 0 };
    Vec v; v.start = (Ptr*)std::malloc(2 * sizeof(Ptr)); v.finish = v.start; v.eos = v.start + 2;
    new (v.finish++) Ptr(&a); new (v.finish++) Ptr(&b);
    Ptr x(&c);
    v.insert_overflow(v.finish, x, 1, true);       // push_back
    if (v.size() != 3 || v.eos - v.start != 4 || v.start[2].p != &c || c.refs != 2 || a.refs != 1 || b.refs != 1) { std::printf("005b3275_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, x, 3, false);   // middle insert of 3
    if (v.size() != 6 || v.eos - v.start != 6 || v.start[0].p != &a || v.start[1].p != &c || v.start[3].p != &c || v.start[4].p != &b || v.start[5].p != &c) { std::printf("005b3275_TEST FAIL\n"); return 1; }
    if (a.refs != 1 || b.refs != 1 || c.refs != 5) { std::printf("005b3275_TEST FAIL\n"); return 1; }
    Vec e; e.start = e.finish = e.eos = 0;
    e.insert_overflow(0, x, 1, true);
    if (e.size() != 1 || e.eos - e.start != 1 || c.refs != 6) { std::printf("005b3275_TEST FAIL\n"); return 1; }
    std::printf("005b3275_TEST PASS\n");
    return 0;
}