// behaviour test for 0x0042C082 hinted insert_unique of map<NUISystem::CComponent*, long>: whatever the hint (begin, end, before, after, far away, equal key) the key ends up once, in order, and the returned position holds it; a well-placed hint inserts without the fallback search
#include <cstdio>
static unsigned long gKeys[64]; static int gCount = 0; static int gFallbacks = 0;
static int InsertAt(int pos, unsigned long k)            // _M_insert: link before position pos
{
    for (int i = gCount; i > pos; --i) gKeys[i] = gKeys[i - 1];
    gKeys[pos] = k; ++gCount; return pos;
}
static int InsertUnique(unsigned long k)                  // the unhinted insert_unique(v).first
{
    ++gFallbacks;
    int i = 0; while (i < gCount && gKeys[i] < k) ++i;
    if (i < gCount && gKeys[i] == k) return i;
    return InsertAt(i, k);
}
static int HintedInsert(int pos, unsigned long v)         // the function under test (pos == gCount is end())
{
    if (pos == 0 && gCount > 0) {                         // begin()
        if (v < gKeys[0]) return InsertAt(0, v);
        if (!(gKeys[0] < v)) return 0;
        int after = 1;
        if (after == gCount) return InsertAt(after, v);
        if (v < gKeys[after]) return InsertAt(after, v);
        return InsertUnique(v);
    }
    if (pos == 0) return InsertUnique(v);                 // empty: size() <= 0
    if (pos == gCount) {                                  // end()
        if (gKeys[gCount - 1] < v) return InsertAt(gCount, v);
        return InsertUnique(v);
    }
    int before = pos - 1;
    bool comp_v_pos = v < gKeys[pos];
    if (comp_v_pos && gKeys[before] < v) return InsertAt(pos, v);
    int after = pos + 1;
    bool comp_pos_v = !comp_v_pos;
    if (!comp_v_pos) comp_pos_v = gKeys[pos] < v;
    if (!comp_v_pos && comp_pos_v && (after == gCount || v < gKeys[after])) return InsertAt(after, v);
    if (comp_v_pos == comp_pos_v) return pos;
    return InsertUnique(v);
}
static bool Sorted() { for (int i = 1; i < gCount; ++i) if (!(gKeys[i - 1] < gKeys[i])) return false; return true; }
int main()
{
    int fail = 0;
    unsigned long seq[] = { 50, 10, 90, 30, 70, 20, 80, 60, 40, 0x80000000UL, 0xFFFFFFF0UL, 30, 50 };
    int hints[] = { 0, 0, 1, 1, 2, 0, 6, 3, 4, 9, 2, 5, 0 };
    for (unsigned int i = 0; i < sizeof(seq) / sizeof(seq[0]); ++i) {
        int h = hints[i] > gCount ? gCount : hints[i];
        int at = HintedInsert(h, seq[i]);
        if (gKeys[at] != seq[i]) fail |= 1;
        if (!Sorted()) fail |= 2;
    }
    if (gCount != 11) fail |= 4;
    int before = gFallbacks;
    HintedInsert(gCount, 0xFFFFFFFFUL);                   // end hint for a new maximum: no fallback
    HintedInsert(0, 1);                                   // begin hint for a new minimum: no fallback
    if (gFallbacks != before || gKeys[0] != 1 || gKeys[gCount - 1] != 0xFFFFFFFFUL) fail |= 8;
    if (fail) { std::printf("0042c082_TEST FAIL %d\n", fail); return 1; }
    std::printf("0042c082_TEST PASS\n");
    return 0;
}