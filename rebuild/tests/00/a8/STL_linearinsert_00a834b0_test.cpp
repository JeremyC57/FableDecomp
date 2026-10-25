// Behaviour test for 0x00A834B0 __linear_insert over (long id, counted node) pairs: a value whose key is below the
// front shifts [first, last) up one slot and lands at the front; otherwise the unguarded backward scan inserts it; the
// counted pointers' reference counts stay balanced (the by-value copies are released).
#include <cstdio>

struct Info { long RefCount; };

struct Ptr {
    int* Data; Info* I;
    Ptr() : Data(0), I(0) { }
    Ptr(int* d, Info* i) : Data(d), I(i) { if (I) ++I->RefCount; }
    Ptr(const Ptr& p) : Data(p.Data), I(p.I) { if (I) ++I->RefCount; }
    Ptr& operator=(const Ptr& p)
    {
        if (I != p.I) {
            if (I) --I->RefCount;
            Data = p.Data; I = p.I;
            if (I) ++I->RefCount;
        }
        return *this;
    }
    ~Ptr() { if (I) --I->RefCount; }
};
struct Entry { long first; Ptr second; };

static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }

static void UnguardedLinearInsert(Entry* last, Entry val)
{
    Entry* next = last - 1;
    while (Less(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(Entry* first, Entry* last, Entry val)
{
    if (Less(val, *first)) {
        for (Entry* p = last; p != first; --p) *p = *(p - 1);    // copy_backward(first, last, last + 1)
        *first = val;
    } else
        UnguardedLinearInsert(last, val);
}

int main()
{
    static int objs[4];
    static Info infos[4];
    for (unsigned int i = 0; i < 4; ++i) infos[i].RefCount = 0;
    Entry a[4];
    long keys[4] = { 5, 9, 12, 7 };
    for (unsigned int i = 0; i < 4; ++i) { a[i].first = keys[i]; a[i].second = Ptr(&objs[i], &infos[i]); }
    int ok = 1;
    LinearInsert(a, a + 3, a[3]);                            // 7 goes between 5 and 9 (scan arm)
    if (a[0].first != 5 || a[1].first != 7 || a[2].first != 9 || a[3].first != 12) ok = 0;
    if (a[1].second.Data != &objs[3] || a[3].second.Data != &objs[2]) ok = 0;
    Entry low; low.first = 1; low.second = Ptr(&objs[0], &infos[0]);
    Entry b[3];
    b[0] = a[0]; b[1] = a[1]; b[2] = low;
    LinearInsert(b, b + 2, b[2]);                            // 1 is below the front (copy_backward arm)
    if (b[0].first != 1 || b[1].first != 5 || b[2].first != 7) ok = 0;
    if (b[2].second.Data != &objs[3]) ok = 0;
    // every live Ptr is counted exactly once: infos[0] by a[0], low, b[0], b[1]; infos[3] by a[1], b[2]
    if (infos[0].RefCount != 4 || infos[3].RefCount != 2 || infos[1].RefCount != 1 || infos[2].RefCount != 1) ok = 0;
    std::printf(ok ? "00a834b0_TEST PASS\n" : "00a834b0_TEST FAIL\n");
    return ok ? 0 : 1;
}