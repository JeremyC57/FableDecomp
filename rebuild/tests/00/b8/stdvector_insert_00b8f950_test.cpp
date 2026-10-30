// Behaviour test for 0x00B8F950 std::vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >::insert(pos, x):
// insert at end constructs in place, insert inside shifts the tail right by one and assigns x at pos, a full vector
// goes to _M_insert_overflow (modelled as a reallocating insert); the return is begin() + the original index and the
// shared count blocks end with the right reference counts.
#include <cstdio>
#include <cstdlib>

static int g_freed = 0;
struct Info { unsigned long RefCount; void* Data; };
struct Entry {
    unsigned long first; void* Data; Info* I;
    Entry() : first(0), Data(0), I(0) { }
    Entry(const Entry& o) : first(o.first), Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    void release() { if (I && --I->RefCount == 0) { ++g_freed; std::free(I); } }
    Entry& operator=(const Entry& o)
    {
        first = o.first;
        if (I != o.I) { release(); Data = o.Data; I = o.I; if (I) ++I->RefCount; }
        return *this;
    }
    ~Entry() { release(); }
};
inline void* operator new(std::size_t, Entry* p) { return p; }

struct Vec {
    Entry* start; Entry* finish; Entry* eos; int overflowCalls;
    explicit Vec(int cap) : overflowCalls(0) { start = finish = (Entry*)std::malloc(sizeof(Entry) * cap); eos = start + cap; }
    void overflow(Entry* pos, const Entry& x)
    {
        ++overflowCalls;
        unsigned int sz = finish - start, cap = sz + (sz > 1 ? sz : 1);
        Entry* ns = (Entry*)std::malloc(sizeof(Entry) * cap); Entry* c = ns;
        for (Entry* p = start; p != pos; ++p, ++c) new (c) Entry(*p);
        new (c) Entry(x); ++c;
        for (Entry* p = pos; p != finish; ++p, ++c) new (c) Entry(*p);
        for (Entry* p = start; p != finish; ++p) p->~Entry();
        std::free(start); start = ns; finish = c; eos = ns + cap;
    }
    Entry* insert(Entry* pos, const Entry& x)
    {
        unsigned int n = pos - start;
        if (finish != eos) {
            if (pos == finish) { new (finish) Entry(x); ++finish; }
            else {
                new (finish) Entry(*(finish - 1)); ++finish;
                Entry x_copy = x;
                Entry* r = finish - 1; Entry* l = finish - 2;
                for (int k = l - pos; k > 0; --k) *--r = *--l;
                *pos = x_copy;
            }
        } else
            overflow(pos, x);
        return start + n;
    }
};

static Entry mk(unsigned long key, Info** out)
{
    Info* i = (Info*)std::malloc(sizeof(Info)); i->RefCount = 1; i->Data = 0;
    Entry e; e.first = key; e.I = i; if (out) *out = i; return e;   // e's copy keeps the count at 1 after the local dies
}

int main()
{
    int bad = 0;
    {
        Info* ia; Info* ib; Info* ic;
        Entry a = mk(10, &ia); Entry b = mk(20, &ib); Entry c = mk(30, &ic);
        ia->RefCount = ib->RefCount = ic->RefCount = 1;
        Vec v(3);
        if (v.insert(v.finish, a) != v.start) bad = 1;                 // at end
        if (v.insert(v.finish, c) != v.start + 1) bad = 1;
        Entry* r = v.insert(v.start + 1, b);                           // inside: shift c right
        if (r != v.start + 1 || v.finish - v.start != 3) bad = 1;
        if (v.start[0].first != 10 || v.start[1].first != 20 || v.start[2].first != 30) bad = 1;
        if (ia->RefCount != 2 || ib->RefCount != 2 || ic->RefCount != 2) bad = 1;
        r = v.insert(v.start, c);                                      // full: overflow path
        if (v.overflowCalls != 1 || v.finish - v.start != 4 || r != v.start || v.start[0].first != 30 || v.start[3].first != 30) bad = 1;
        if (ic->RefCount != 3) bad = 1;
        for (Entry* p = v.start; p != v.finish; ++p) p->~Entry();
        std::free(v.start);
    }
    if (g_freed != 3) bad = 1;
    if (bad) { std::printf("00b8f950_TEST FAIL\n"); return 1; }
    std::printf("00b8f950_TEST PASS\n");
    return 0;
}