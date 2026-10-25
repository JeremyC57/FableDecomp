// Behaviour test for 0x00BEF2B0 __linear_insert over ((long, long) key, texture decal) entries: lexicographic key order;
// a value below the front shifts [first, last) up one slot and lands at the front, otherwise the unguarded backward
// scan inserts it; the decal (two longs and its blend list) travels with its key.
#include <cstdio>

struct Key { long a, b; };
static bool KeyLess(const Key& x, const Key& y) { return x.a < y.a || (!(y.a < x.a) && x.b < y.b); }
struct Decal { long OriginalTextureIndex, DecalID; int blends[2]; unsigned int blendCount; };
struct Entry { Key first; Decal second; };

static bool Less(const Entry& x, const Entry& y) { return KeyLess(x.first, y.first); }
static void UnguardedLinearInsert(Entry* last, Entry val)
{
    Entry* next = last - 1;
    while (Less(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(Entry* first, Entry* last, Entry val)
{
    if (Less(val, *first)) {
        for (Entry* p = last; p != first; --p) *p = *(p - 1);
        *first = val;
    } else
        UnguardedLinearInsert(last, val);
}

static Entry Make(long a, long b, long tex)
{
    Entry e; e.first.a = a; e.first.b = b; e.second.OriginalTextureIndex = tex; e.second.DecalID = tex * 10;
    e.second.blends[0] = (int)tex; e.second.blends[1] = (int)tex + 1; e.second.blendCount = 2;
    return e;
}

int main()
{
    Entry v[5];
    v[0] = Make(1, 5, 100); v[1] = Make(2, 1, 200); v[2] = Make(2, 7, 300); v[3] = Make(2, 3, 400);
    int ok = 1;
    LinearInsert(v, v + 3, v[3]);                            // (2,3) lands between (2,1) and (2,7): the second word decides
    if (v[1].first.b != 1 || v[2].first.b != 3 || v[3].first.b != 7) ok = 0;
    if (v[2].second.OriginalTextureIndex != 400 || v[2].second.DecalID != 4000 || v[3].second.blends[1] != 301) ok = 0;
    v[4] = Make(0, 9, 500);
    LinearInsert(v, v + 4, v[4]);                            // (0,9) is below the front
    if (v[0].first.a != 0 || v[0].second.OriginalTextureIndex != 500 || v[1].first.a != 1 || v[4].first.b != 7) ok = 0;
    if (v[4].second.blendCount != 2 || v[4].second.blends[0] != 300) ok = 0;
    std::printf(ok ? "00bef2b0_TEST PASS\n" : "00bef2b0_TEST FAIL\n");
    return ok ? 0 : 1;
}