#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour model of the list -> back_insert_iterator<vector<CThing*> > filter (0x004ADCB0): every listed thing whose
// flag byte is set is pushed onto the vector (growing through _M_insert_overflow); the output iterator is returned.
class CThing { public: bool Flag; int id; };
struct ListNode { ListNode* next; ListNode* prev; CThing* data; };
struct ListIt { ListNode* node; CThing*& operator*() const { return node->data; } ListIt& operator++() { node = node->next; return *this; } bool operator!=(const ListIt& o) const { return node != o.node; } };
static int g_overflows = 0;
struct Vec {
    CThing** start; CThing** finish; CThing** eos;
    void overflow(CThing** pos, CThing* const& x)
    {
        ++g_overflows;
        unsigned int old = (unsigned int)(finish - start), len = old + (old < 1 ? 1 : old);
        CThing** ns = (CThing**)std::malloc(len * sizeof(CThing*));
        unsigned int head = (unsigned int)(pos - start);
        if (head) std::memmove(ns, start, head * sizeof(CThing*));
        ns[head] = x;
        if (start) std::free(start);
        start = ns; finish = ns + head + 1; eos = ns + len;
    }
    void push_back(CThing* const& x) { if (finish != eos) { *finish = x; ++finish; } else overflow(finish, x); }
};
struct BackInserter {
    Vec* container;
    BackInserter& operator=(CThing* const& v) { container->push_back(v); return *this; }
    BackInserter& operator*() { return *this; }
    BackInserter& operator++(int) { return *this; }
};
BackInserter CopyIfFlagSet(ListIt first, ListIt last, BackInserter result)
{
    for ( ; first != last; ++first) {
        CThing* thing = *first;
        if (thing->Flag)
            *result++ = thing;
    }
    return result;
}

int main()
{
    CThing t[5] = { { true, 0 }, { false, 1 }, { true, 2 }, { true, 3 }, { false, 4 } };
    ListNode head, n[5];
    ListNode* prev = &head;
    for (int i = 0; i < 5; ++i) { n[i].data = &t[i]; prev->next = &n[i]; n[i].prev = prev; prev = &n[i]; }
    prev->next = &head; head.prev = prev;
    Vec v = { 0, 0, 0 };
    BackInserter out = { &v };
    ListIt b = { head.next }, e = { &head };
    BackInserter r = CopyIfFlagSet(b, e, out);
    if (r.container != &v || v.finish - v.start != 3 || v.start[0] != &t[0] || v.start[1] != &t[2] || v.start[2] != &t[3]) { std::printf("004adcb0_TEST FAIL\n"); return 1; }
    if (g_overflows != 3) { std::printf("004adcb0_TEST FAIL\n"); return 1; }   // capacities 1, 2, 4
    ListIt e2 = { &head };
    BackInserter r2 = CopyIfFlagSet(e2, e2, out);                                  // empty range: iterator returned unchanged
    if (r2.container != &v || v.finish - v.start != 3) { std::printf("004adcb0_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("004adcb0_TEST PASS\n");
    return 0;
}