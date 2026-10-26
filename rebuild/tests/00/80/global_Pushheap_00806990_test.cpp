// behaviour test for 0x00806990 _Push_heap<CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate>
#include <cstdio>
typedef unsigned int u32;
static u32 ror13(u32 v) { return (v >> 13) | (v << 19); }
struct CRandomSeed {
    u32 Seed;
    u32 Random() { Seed = Seed * 0x24a1 + 0x24df; Seed = ror13(Seed); return Seed; }
};
static CRandomSeed g_seed;
class CThing { public: int id; };
struct ThingPointer {
    CThing* item;
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* p) { item = p; }
    ThingPointer& operator=(const ThingPointer& p) { SetPItem(p.GetPItem()); return *this; }
};
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        if (g_seed.Random() & 1)
            return false;
        return true;
    }
};
static void _Push_heap(ThingPointer* first, long hole, long top, ThingPointer value, CDoNotUseBinaryPredicate comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    CThing things[8];
    ThingPointer heap[8];
    for (int i = 0; i < 8; ++i) { things[i].id = i; heap[i].item = &things[i]; }
    CDoNotUseBinaryPredicate c; c.m_tag = 0;
    // find a seed whose first coin is "false" (odd): the value stays in the hole
    u32 s = 1;
    for (;; ++s) { CRandomSeed t; t.Seed = s; if (t.Random() & 1) break; }
    g_seed.Seed = s;
    CThing extra; extra.id = 99;
    ThingPointer v; v.item = &extra;
    _Push_heap(heap, 7, 0, v, c);
    if (heap[7].item != &extra || heap[3].item != &things[3]) { std::printf("00806990_TEST FAIL\n"); return 1; }
    // a seed whose first two coins are "true" (even): the value climbs 7 -> 3 -> 1
    for (s = 1;; ++s) { CRandomSeed t; t.Seed = s; u32 a = t.Random(); u32 b = t.Random(); u32 d = t.Random(); if (!(a & 1) && !(b & 1) && (d & 1)) break; }
    for (int i = 0; i < 8; ++i) heap[i].item = &things[i];
    g_seed.Seed = s;
    _Push_heap(heap, 7, 0, v, c);
    if (heap[1].item != &extra || heap[3].item != &things[1] || heap[7].item != &things[3] || heap[0].item != &things[0]) { std::printf("00806990_TEST FAIL\n"); return 1; }
    // hole == top: no coin is drawn
    g_seed.Seed = 1234;
    _Push_heap(heap, 2, 2, v, c);
    if (g_seed.Seed != 1234 || heap[2].item != &extra) { std::printf("00806990_TEST FAIL\n"); return 1; }
    std::printf("00806990_TEST PASS\n");
    return 0;
}