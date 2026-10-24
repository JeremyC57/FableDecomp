// Behaviour test for 0x00757870 CWoundMorphs::GetPRandomWoundForLocation(EBodyLocation, unsigned long&, long&) const
#include <cstdio>

typedef unsigned int u32;
enum EBodyLocation { LOC0 = 0, LOC1 = 1, LOC2 = 2, LOC3 = 3, LOC5 = 5 };

static int g_sortCalls = 0;

u32 GFROR13(u32 value) { return (value >> 13) | (value << 19); }

inline u32 GFRandom(u32 range, u32* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

struct CEntry { EBodyLocation Location; int Payload[9]; };
struct TWoundPair { EBodyLocation first; CEntry second; };
struct TCompare { bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; } };

void sort(TWoundPair* first, TWoundPair* last, TCompare comp)
{
    ++g_sortCalls;
    for (TWoundPair* i = first; i != last; ++i)
        for (TWoundPair* j = i; j != first && comp(*j, *(j - 1)); --j) {
            TWoundPair t = *j; *j = *(j - 1); *(j - 1) = t;
        }
}

struct TMap {
    TWoundPair* First;
    TWoundPair* Last;
    TWoundPair* EndOfStorage;
    TCompare    Compare;
    bool        Dirty;
    TWoundPair* LowerBound(const EBodyLocation& key) const
    {
        TWoundPair* p = First;
        while (p != Last && p->first < key) ++p;
        return p;
    }
    TWoundPair* UpperBound(const EBodyLocation& key) const
    {
        TWoundPair* p = First;
        while (p != Last && !(key < p->first)) ++p;
        return p;
    }
};

struct CWoundMorphs {
    mutable TMap Morphs;
    const CEntry* GetPRandomWoundForLocation(EBodyLocation location, u32& seed, long& index) const;
};

const CEntry* CWoundMorphs::GetPRandomWoundForLocation(EBodyLocation location, u32& seed, long& index) const
{
    const CEntry* result = 0;
    if (Morphs.Dirty) {
        sort(Morphs.First, Morphs.Last, Morphs.Compare);
        Morphs.Dirty = false;
    }
    long count = Morphs.UpperBound(location) - Morphs.LowerBound(location);
    if (count != 0) {
        u32 r = GFRandom(count, &seed);
        const TWoundPair* entry = Morphs.LowerBound(location) + r;
        result = &entry->second;
        index = entry - Morphs.First;
    }
    return result;
}

int main()
{
    TWoundPair data[6];
    EBodyLocation keys[6] = { LOC2, LOC1, LOC2, LOC5, LOC2, LOC0 };
    for (int i = 0; i < 6; ++i) { data[i].first = keys[i]; data[i].second.Location = keys[i]; data[i].second.Payload[0] = 100 + i; }
    CWoundMorphs m;
    m.Morphs.First = data; m.Morphs.Last = data + 6; m.Morphs.EndOfStorage = data + 6; m.Morphs.Dirty = true;
    int ok = 1;
    u32 seed = 12345;
    long index = -1;
    // three LOC2 entries -> after the sort they are data[2..4]
    const CEntry* e = m.GetPRandomWoundForLocation(LOC2, seed, index);
    u32 s = 12345; s = s * 0x24a1 + 0x24df; s = GFROR13(s);
    long want = 2 + (long)(s % 3);
    if (g_sortCalls != 1 || m.Morphs.Dirty) ok = 0;
    if (seed != s || index != want || e != &data[want].second || e->Location != LOC2) ok = 0;
    // second call: no re-sort, seed advances again
    e = m.GetPRandomWoundForLocation(LOC5, seed, index);
    s = s * 0x24a1 + 0x24df; s = GFROR13(s);
    if (g_sortCalls != 1 || seed != s || index != 5 || e != &data[5].second) ok = 0;
    // absent location: null result, seed and index untouched
    index = 77;
    e = m.GetPRandomWoundForLocation(LOC3, seed, index);
    if (e != 0 || index != 77 || seed != s) ok = 0;
    if (!ok) { std::printf("00757870_TEST FAIL\n"); return 1; }
    std::printf("00757870_TEST PASS\n");
    return 0;
}