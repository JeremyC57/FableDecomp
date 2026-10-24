#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour copy of STLport vector<CShopItemDef>::operator=: realloc branch, shrink branch (assign + destroy surplus),
// grow-within-capacity branch (assign + copy-construct tail). The element counts live objects.
static int g_live = 0;
struct SItem {
    long Type; float Quantity;
    SItem(long t, float q) : Type(t), Quantity(q) { ++g_live; }
    SItem(const SItem& o) : Type(o.Type), Quantity(o.Quantity) { ++g_live; }
    SItem& operator=(const SItem& o) { Type = o.Type; Quantity = o.Quantity; return *this; }
    ~SItem() { --g_live; }
};
struct SVec {
    SItem* start; SItem* finish; SItem* eos;
    SVec() : start(0), finish(0), eos(0) { }
    unsigned size() const { return unsigned(finish - start); }
    unsigned capacity() const { return unsigned(eos - start); }
    void reserve_raw(unsigned n) { start = finish = (SItem*)std::malloc(n * sizeof(SItem)); eos = start + n; }
    void push(const SItem& v) { new (finish) SItem(v); ++finish; }
    void clear() { for (SItem* p = start; p != finish; ++p) p->~SItem(); std::free(start); start = finish = eos = 0; }
    SItem* allocate_and_copy(unsigned n, const SItem* f, const SItem* l)
    {
        SItem* r = n ? (SItem*)std::malloc(n * sizeof(SItem)) : 0;
        SItem* c = r; for (; f != l; ++f, ++c) new (c) SItem(*f);
        return r;
    }
    SVec& assign(const SVec& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                SItem* tmp = allocate_and_copy(xlen, x.start, x.finish);
                clear();
                start = tmp; eos = start + xlen;
            } else if (size() >= xlen) {
                SItem* i = start; for (const SItem* f = x.start; f != x.finish; ++f, ++i) *i = *f;
                for (SItem* p = i; p != finish; ++p) p->~SItem();
            } else {
                unsigned n = size();
                SItem* i = start; for (const SItem* f = x.start; f != x.start + n; ++f, ++i) *i = *f;
                SItem* c = finish; for (const SItem* f = x.start + n; f != x.finish; ++f, ++c) new (c) SItem(*f);
            }
            finish = start + xlen;
        }
        return *this;
    }
};
int main()
{
    bool ok = true;
    SVec a, b;
    a.reserve_raw(4); for (int i = 0; i < 3; ++i) a.push(SItem(i + 1, 1.5f * i));
    b.assign(a);                                                     // realloc branch
    if (b.size() != 3 || b.capacity() != 3 || b.start[2].Type != 3 || g_live != 6) ok = false;
    SVec c; c.reserve_raw(8); for (int i = 0; i < 5; ++i) c.push(SItem(100 + i, 0.0f));
    c.assign(a);                                                     // shrink branch
    if (c.size() != 3 || c.capacity() != 8 || c.start[0].Type != 1 || c.start[2].Quantity != 3.0f || g_live != 9) ok = false;
    SVec d; d.reserve_raw(8); d.push(SItem(7, 7.0f));
    d.assign(a);                                                     // grow-within-capacity branch
    if (d.size() != 3 || d.capacity() != 8 || d.start[0].Type != 1 || d.start[1].Type != 2 || d.start[2].Type != 3 || g_live != 12) ok = false;
    d.assign(d);                                                     // self-assignment is a no-op
    if (d.size() != 3 || g_live != 12) ok = false;
    a.clear(); b.clear(); c.clear(); d.clear();
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004e9ad3_TEST FAIL\n"); return 1; }
    std::printf("004e9ad3_TEST PASS\n");
    return 0;
}