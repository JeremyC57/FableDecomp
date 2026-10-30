// Behaviour test for std::vector<C3DGroup2>::_M_fill_insert (0x00AE82C0): the STLport fill-insert over a copy of the
// logic, for the "tail longer than n" arm, the "tail not longer than n" arm and the overflow arm.
#include <cstdio>
#include <cstdlib>
#include <new>
struct Group {                       // stands for C3DGroup2: a bone index plus an owned blend block
    long BoneIndex;
    int* Blends;
    Group() : BoneIndex(0), Blends(0) {}
    Group(long b) : BoneIndex(b), Blends((int*)std::malloc(sizeof(int))) { *Blends = (int)b * 10; }
    Group(const Group& o) : BoneIndex(o.BoneIndex), Blends((int*)std::malloc(sizeof(int))) { *Blends = *o.Blends; }
    Group& operator=(const Group& o) { BoneIndex = o.BoneIndex; *Blends = *o.Blends; return *this; }
    ~Group() { std::free(Blends); }
};
struct Vec {
    Group* start; Group* finish; Group* eos;
    Vec(unsigned cap) { start = finish = (Group*)std::malloc(cap * sizeof(Group)); eos = start + cap; }
    ~Vec() { for (Group* p = start; p != finish; ++p) p->~Group(); std::free(start); }
    void push(long b) { new (finish) Group(b); ++finish; }
    void insert_overflow(Group* pos, const Group& x, unsigned n)
    {
        unsigned old = finish - start, len = old + (old > n ? old : n);
        Group* ns = (Group*)std::malloc(len * sizeof(Group)); Group* nf = ns;
        for (Group* p = start; p != pos; ++p, ++nf) new (nf) Group(*p);
        for (unsigned k = 0; k < n; ++k, ++nf) new (nf) Group(x);
        for (Group* p = pos; p != finish; ++p, ++nf) new (nf) Group(*p);
        for (Group* p = start; p != finish; ++p) p->~Group();
        std::free(start); start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(Group* position, unsigned n, const Group& x)
    {
        if (n != 0) {
            if (unsigned(eos - finish) >= n) {
                Group x_copy = x;
                const unsigned elems_after = finish - position;
                Group* old_finish = finish;
                if (elems_after > n) {
                    Group* d = finish;
                    for (Group* p = finish - n; p != old_finish; ++p, ++d) new (d) Group(*p);
                    finish += n;
                    Group* r = old_finish; Group* l = old_finish - n;
                    for (int k = l - position; k > 0; --k) *--r = *--l;
                    for (Group* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    Group* d = finish;
                    for (unsigned k = n - elems_after; k > 0; --k, ++d) new (d) Group(x_copy);
                    finish += n - elems_after;
                    d = finish;
                    for (Group* p = position; p != old_finish; ++p, ++d) new (d) Group(*p);
                    finish += elems_after;
                    for (Group* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                insert_overflow(position, x, n);
        }
    }
};
static bool same(const Vec& v, const long* want, unsigned n)
{
    if (unsigned(v.finish - v.start) != n) return false;
    for (unsigned i = 0; i < n; ++i)
        if (v.start[i].BoneIndex != want[i] || *v.start[i].Blends != want[i] * 10) return false;
    return true;
}
int main()
{
    Group x(7);
    Vec a(16); a.push(1); a.push(2); a.push(3); a.push(4); a.push(5);
    a.fill_insert(a.start + 1, 2, x);                       // tail 4 > n 2
    long wa[] = { 1, 7, 7, 2, 3, 4, 5 };
    Vec b(16); b.push(1); b.push(2); b.push(3);
    b.fill_insert(b.start + 2, 3, x);                       // tail 1 <= n 3
    long wb[] = { 1, 2, 7, 7, 7, 3 };
    Vec c(3); c.push(1); c.push(2); c.push(3);
    c.fill_insert(c.start + 1, 2, x);                       // overflow
    long wc[] = { 1, 7, 7, 2, 3 };
    Vec d(4); d.push(9);
    d.fill_insert(d.start, 0, x);                           // n == 0: nothing
    long wd[] = { 9 };
    if (!same(a, wa, 7) || !same(b, wb, 6) || !same(c, wc, 5) || !same(d, wd, 1) || c.eos - c.start != 6) {
        std::printf("00ae82c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ae82c0_TEST PASS\n");
    return 0;
}