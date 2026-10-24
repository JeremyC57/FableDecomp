// behaviour test for 0x00806910 __unguarded_linear_insert<CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate>
#include <cstdio>
typedef unsigned int u32;
static u32 ror13(u32 v) { return (v >> 13) | (v << 19); }
struct CRandomSeed {
    u32 Seed;
    u32 Random() { Seed = Seed * 0x24a1 + 0x24df; Seed = ror13(Seed); return Seed; }
};
static CRandomSeed g_seed;
struct CThing { int id; };
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
static void __unguarded_linear_insert(ThingPointer* last, ThingPointer value, CDoNotUseBinaryPredicate comp)
{
    ThingPointer* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
int main()
{
    CThing things[6];
    ThingPointer arr[6];
    for (int i = 0; i < 6; ++i) { things[i].id = i; arr[i].item = &things[i]; }
    CDoNotUseBinaryPredicate c; c.m_tag = 0;
    // two "true" coins then a "false" one: the value moves two slots left
    u32 s;
    for (s = 1;; ++s) { CRandomSeed t; t.Seed = s; u32 a = t.Random(); u32 b = t.Random(); u32 d = t.Random(); if (!(a & 1) && !(b & 1) && (d & 1)) break; }
    g_seed.Seed = s;
    CThing extra; extra.id = 77;
    ThingPointer v; v.item = &extra;
    __unguarded_linear_insert(&arr[5], v, c);
    if (arr[3].item != &extra || arr[4].item != &things[3] || arr[5].item != &things[4] || arr[2].item != &things[2]) { std::printf("00806910_TEST FAIL\n"); return 1; }
    // first coin "false": stored in place
    for (s = 1;; ++s) { CRandomSeed t; t.Seed = s; if (t.Random() & 1) break; }
    g_seed.Seed = s;
    __unguarded_linear_insert(&arr[1], v, c);
    if (arr[1].item != &extra || arr[0].item != &things[0]) { std::printf("00806910_TEST FAIL\n"); return 1; }
    std::printf("00806910_TEST PASS\n");
    return 0;
}