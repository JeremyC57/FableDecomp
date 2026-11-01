// Behaviour test for 0x00A6BCD0 vector<CIVCountedPointer<T> >::_M_fill_insert: in-place slide + fill with refcounts
#include <cstdio>
#include <cstdlib>

struct Pointee {
    unsigned int IVRefCount; int deleted;
    void Delete() { deleted = 1; }
};
struct CP {
    Pointee* Object;
    CP() : Object(0) { }
    CP(Pointee* p) : Object(p) { if (Object) ++Object->IVRefCount; }
    CP(const CP& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CP() { if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); Object = 0; } }
    CP& operator=(const CP& o)
    {
        if (Object != o.Object) {
            if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};
static void construct(CP* p, const CP& v) { p->Object = v.Object; if (p->Object) ++p->Object->IVRefCount; }

struct Vec {
    CP* start; CP* finish; CP* eos; int overflow;
    void fill_insert(CP* pos, unsigned int n, const CP& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                CP x_copy = x;
                unsigned int after = finish - pos;
                CP* old_finish = finish;
                if (after > n) {
                    for (CP *s = finish - n, *d = finish; s != finish; ++s, ++d) construct(d, *s);
                    finish += n;
                    CP* r = old_finish; for (CP* l = old_finish - n; l != pos; ) *--r = *--l;
                    for (CP* f = pos; f != pos + n; ++f) *f = x_copy;
                } else {
                    CP* c = finish; for (unsigned int k = n - after; k > 0; --k, ++c) construct(c, x_copy);
                    finish += n - after;
                    for (CP *s = pos, *d = finish; s != old_finish; ++s, ++d) construct(d, *s);
                    finish += after;
                    for (CP* f = pos; f != old_finish; ++f) *f = x_copy;
                }
            } else
                overflow = 1;
        }
    }
};

int main()
{
    Pointee a = { 0, 0 }, b = { 0, 0 }, x = { 0, 0 };
    CP* raw = (CP*)std::malloc(sizeof(CP) * 8);
    construct(&raw[0], CP(&a)); construct(&raw[1], CP(&b)); construct(&raw[2], CP(&a));
    Vec v = { raw, raw + 3, raw + 8, 0 };
    CP xp(&x);
    v.fill_insert(raw + 1, 1, xp);                               // after(2) > n(1): slide path
    int ok = v.finish == raw + 4 && raw[0].Object == &a && raw[1].Object == &x && raw[2].Object == &b && raw[3].Object == &a
          && x.IVRefCount == 2 && a.IVRefCount == 2 && b.IVRefCount == 1;
    v.fill_insert(raw + 3, 3, xp);                               // after(1) <= n(3): fill_n path
    ok = ok && v.finish == raw + 7 && raw[3].Object == &x && raw[4].Object == &x && raw[5].Object == &x && raw[6].Object == &a
            && x.IVRefCount == 5 && a.IVRefCount == 2;
    v.fill_insert(raw, 4, xp);                                   // no room: overflow path
    ok = ok && v.overflow == 1 && x.IVRefCount == 5;
    v.overflow = 0; v.fill_insert(raw, 0, xp);
    ok = ok && v.overflow == 0 && !x.deleted;
    if (!ok) { std::printf("00a6bcd0_TEST FAIL\n"); return 1; }
    std::printf("00a6bcd0_TEST PASS\n");
    return 0;
}