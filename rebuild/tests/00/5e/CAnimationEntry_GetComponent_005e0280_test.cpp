// behaviour test for 0x005E0280 CAnimationEntry::GetComponent<CAnimComponentTransitionInTime>: the entry's own component when its map holds type 1, else the default map's (when it holds the type), else the default map's end slot; a null map goes straight to the defaults
#include <cstdio>
struct Pair { int first; void* second; };
struct Map { Pair* b; Pair* e; };
static const Pair* LowerBound(const Map& m, int key)
{
    const Pair* lo = m.b; int len = (int)(m.e - m.b);
    while (len > 0) { int half = len >> 1; const Pair* mid = lo + half; if (mid->first < key) { lo = mid + 1; len = len - half - 1; } else len = half; }
    return lo;
}
static void* Get(const Map* own, const Map& def, int type)
{
    const Pair* it = 0;
    if (own) { it = LowerBound(*own, type); if (it == own->e || type < it->first) it = own->e; }
    if (!own || it == own->e) {
        const Pair* f = LowerBound(def, type);
        it = (f != def.e && !(type < f->first)) ? f : def.e;
    }
    return it->second;
}
int main()
{
    const int T = 1;
    int a = 1, b = 2, c = 3, z = 9;
    Pair ownP[3] = { { T - 1, &c }, { T, &a }, { T + 1, &c } };
    Pair ownQ[3] = { { T - 1, &c }, { T + 1, &c }, { 0, &z } };   // third slot = end sentinel storage
    Pair defP[2] = { { T, &b }, { 0, &z } };
    Pair defQ[2] = { { T + 5, &c }, { 0, &z } };
    Map own = { ownP, ownP + 3 }, miss = { ownQ, ownQ + 2 }, def = { defP, defP + 1 }, defMiss = { defQ, defQ + 1 };
    int fails = 0;
    if (Get(&own, def, T) != &a) fails |= 1;
    if (Get(&miss, def, T) != &b) fails |= 2;
    if (Get(0, def, T) != &b) fails |= 4;
    if (Get(&miss, defMiss, T) != &z) fails |= 8;          // default end slot
    if (fails) { std::printf("005e0280_TEST FAIL %d\n", fails); return 1; }
    std::printf("005e0280_TEST PASS\n");
    return 0;
}