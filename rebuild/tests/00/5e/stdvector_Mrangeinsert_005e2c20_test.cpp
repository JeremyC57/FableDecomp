// Behaviour test for vector<pair<unsigned long, CAnimationEntry> >::_M_range_insert (0x005E2C20): the three arms
// (elems_after > n, elems_after <= n, reallocation) through a copy of the model on a counted element.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct Entry {
    unsigned int key; int value;
    Entry(unsigned int k, int v) : key(k), value(v) { ++g_live; }
    Entry(const Entry& o) : key(o.key), value(o.value) { ++g_live; }
    Entry& operator=(const Entry& o) { key = o.key; value = o.value; return *this; }
    ~Entry() { --g_live; }
};

static Entry* UninitCopy(const Entry* first, const Entry* last, Entry* result)
{
    for (; first != last; ++first, ++result) new (result) Entry(*first);
    return result;
}
static Entry* CopyBackward(Entry* first, Entry* last, Entry* result)
{
    for (int n = int(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static Entry* Copy(const Entry* first, const Entry* last, Entry* result)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}

struct EntryVector {
    Entry* start; Entry* finish; Entry* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void RangeInsert(Entry* position, const Entry* first, const Entry* last)
    {
        if (first == last) return;
        unsigned int n = (unsigned int)(last - first);
        if ((unsigned int)(eos - finish) >= n) {
            const unsigned int elems_after = (unsigned int)(finish - position);
            Entry* old_finish = finish;
            if (elems_after > n) {
                UninitCopy(finish - n, finish, finish);
                finish += n;
                CopyBackward(position, old_finish - n, old_finish);
                Copy(first, last, position);
            } else {
                const Entry* mid = first + elems_after;
                UninitCopy(mid, last, finish);
                finish += n - elems_after;
                UninitCopy(position, old_finish, finish);
                finish += elems_after;
                Copy(first, mid, position);
            }
        } else {
            const unsigned int old_size = size();
            const unsigned int len = old_size + (old_size < n ? n : old_size);
            Entry* new_start = len != 0 ? (Entry*)std::malloc(len * sizeof(Entry)) : 0;
            Entry* new_finish = UninitCopy(start, position, new_start);
            new_finish = UninitCopy(first, last, new_finish);
            new_finish = UninitCopy(position, finish, new_finish);
            for (Entry* p = start; p != finish; ++p) p->~Entry();
            if (start) std::free(start);
            start = new_start; finish = new_finish; eos = new_start + len;
        }
    }
};

static bool Keys(const EntryVector& v, const unsigned int* keys, unsigned int n)
{
    if (v.size() != n) return false;
    for (unsigned int i = 0; i < n; ++i) if (v.start[i].key != keys[i]) return false;
    return true;
}

int main()
{
    EntryVector v;
    v.start = (Entry*)std::malloc(10 * sizeof(Entry));
    for (unsigned int i = 0; i < 5; ++i) new (v.start + i) Entry(i, int(i));   // 0 1 2 3 4, capacity 10
    v.finish = v.start + 5; v.eos = v.start + 10;
    Entry* src = (Entry*)std::malloc(4 * sizeof(Entry));
    for (unsigned int i = 0; i < 4; ++i) new (src + i) Entry(100 + i, 0);

    v.RangeInsert(v.start + 1, src, src + 2);         // elems_after 4 > 2
    const unsigned int k1[] = { 0, 100, 101, 1, 2, 3, 4 };
    if (!Keys(v, k1, 7)) { std::printf("005e2c20_TEST FAIL\n"); return 1; }
    v.RangeInsert(v.start + 5, src, src + 3);         // elems_after 2 <= 3
    const unsigned int k2[] = { 0, 100, 101, 1, 2, 100, 101, 102, 3, 4 };
    if (!Keys(v, k2, 10) || v.eos - v.start != 10) { std::printf("005e2c20_TEST FAIL\n"); return 1; }
    v.RangeInsert(v.start, src + 3, src + 4);         // full: reallocate to 10 + 10
    const unsigned int k3[] = { 103, 0, 100, 101, 1, 2, 100, 101, 102, 3, 4 };
    if (!Keys(v, k3, 11) || v.eos - v.start != 20) { std::printf("005e2c20_TEST FAIL\n"); return 1; }
    v.RangeInsert(v.start, src, src);                 // empty range: no change
    if (!Keys(v, k3, 11) || g_live != 15) { std::printf("005e2c20_TEST FAIL\n"); return 1; }
    std::printf("005e2c20_TEST PASS\n");
    return 0;
}