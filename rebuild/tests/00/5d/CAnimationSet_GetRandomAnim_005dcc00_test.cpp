// Behaviour test for CAnimationSet::GetRandomAnim (0x005DCC00), linked against the compiled row object.
// Checks, from retail's disassembly: the walk starts at FindFirstAnimWithName's result and stops at the first key change or at
// Anims.end(); the filter (vtable slot 0) is asked once per entry of the run, in order; an accepted entry whose BankIndex equals
// index_to_ignore is remembered (the last one wins) instead of collected; with candidates, seed = GFROR13(seed * 0x24A1 + 0x24DF)
// and the result is candidates[seed % count]; without candidates the remembered entry (or Anims.end()->second) is returned and
// seed is left alone; more than 5 candidates exercises the vector's grow path.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include <stdio.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"
#include "engine/GFRandom.h"

// The 9 map pairs, 5 words each (0x14): key, then CAnimationEntry (BankIndex, AnimName, GroupName, Components).
static unsigned long g_ents[9 * 5];
static unsigned long g_set[8];                       // CAnimationSet storage: Anims start / finish / end-of-storage, ...
static const unsigned long* g_first;                 // what FindFirstAnimWithName returns
static int g_findCalls;

long FABLE_FASTCALL GFROR13(unsigned long value) { return (long)((value >> 13) | (value << 19)); }   // retail: ror [esp], 0Dh

CAnimationSet::TAnimMap::const_iterator CAnimationSet::FindFirstAnimWithName(const CCharString&) const
{
    ++g_findCalls;
    return (TAnimMap::const_iterator)g_first;
}

class CTestAnimFilter : public CAnimFilterBase {
public:
    long Accept[32];
    const CAnimationEntry* Seen[32];
    int NSeen;
    CTestAnimFilter() : NSeen(0) { for (int i = 0; i < 32; ++i) Accept[i] = 0; }
    virtual bool operator()(const CAnimationEntry& entry) const
    {
        CTestAnimFilter* self = const_cast<CTestAnimFilter*>(this);
        if (self->NSeen < 32) self->Seen[self->NSeen] = &entry;
        ++self->NSeen;
        return Accept[entry.BankIndex] != 0;
    }
    virtual CAnimFilterBase* Clone() const { return 0; }
};

static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static const CAnimationEntry* Entry(int i) { return (const CAnimationEntry*)&g_ents[i * 5 + 1]; }
static unsigned long Next(unsigned long seed) { return (unsigned long)GFROR13(seed * 0x24A1 + 0x24DF); }

// keys 3 | 7 x7 (entries 1..7) | 9; BankIndex = banks[i].
static void Build(const long* banks, int count)
{
    static const unsigned long keys[9] = { 3, 7, 7, 7, 7, 7, 7, 7, 9 };
    for (int i = 0; i < 9; ++i) {
        g_ents[i * 5] = keys[i]; g_ents[i * 5 + 1] = (unsigned long)banks[i];
        g_ents[i * 5 + 2] = g_ents[i * 5 + 3] = g_ents[i * 5 + 4] = 0;
    }
    g_set[0] = (unsigned long)&g_ents[0]; g_set[1] = (unsigned long)&g_ents[count * 5]; g_set[2] = g_set[1];
    g_first = &g_ents[5];
}

static const CAnimationEntry& Run(CTestAnimFilter& f, unsigned long& seed, long ignore)
{
    unsigned long dummyName = 0;
    const CAnimationSet* set = (const CAnimationSet*)g_set;
    return set->GetRandomAnim(*(const CCharString*)&dummyName, seed, f, ignore);
}

int main()
{
    static const long banks[9] = { 1, 10, 11, 12, 13, 14, 15, 16, 20 };
    Build(banks, 9);
    // A: all 7 of the run accepted, none ignored (7 > reserve(5): grow path); the key-9 entry is never asked.
    {
        CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
        unsigned long seed = 12345, want = Next(12345);
        const CAnimationEntry& r = Run(f, seed, -1);
        Check(seed == want, "A seed advanced by GFRandom");
        Check(&r == Entry(1 + want % 7), "A picks candidates[seed % 7]");
        Check(f.NSeen == 7, "A filter asked once per entry of the run");
        int inOrder = 1; for (int i = 0; i < 7 && i < f.NSeen; ++i) inOrder &= f.Seen[i] == Entry(1 + i);
        Check(inOrder, "A filter sees &pair.second in map order");
        Check(g_findCalls == 1, "A FindFirstAnimWithName called once");
    }
    // B: bank 12 (entry 3) ignored: 6 candidates, entry 3 never returned for a sweep of seeds.
    {
        static const int map6[6] = { 1, 2, 4, 5, 6, 7 };
        int ok = 1;
        for (unsigned long s0 = 0; s0 < 40; ++s0) {
            CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
            unsigned long seed = s0 * 7919u, want = Next(seed);
            const CAnimationEntry& r = Run(f, seed, 12);
            ok &= seed == want && &r == Entry(map6[want % 6]);
        }
        Check(ok, "B ignored bank skipped, candidates[seed % 6]");
    }
    // C: only bank 12 accepted and it is the ignored one: return it, seed untouched.
    {
        CTestAnimFilter f; f.Accept[12] = 1;
        unsigned long seed = 99;
        const CAnimationEntry& r = Run(f, seed, 12);
        Check(&r == Entry(3) && seed == 99, "C only the ignored entry: returned, no GFRandom");
    }
    // D: nothing accepted: Anims.end()->second, seed untouched.
    {
        CTestAnimFilter f;
        unsigned long seed = 5;
        const CAnimationEntry& r = Run(f, seed, -1);
        Check((const char*)&r == (const char*)g_set[1] + 4 && seed == 5, "D no candidate: end()->second");
        Check(f.NSeen == 7, "D filter still asked for the whole run");
    }
    // E: the ignored bank rejected by the filter is not remembered; the one accepted other entry is picked (count 1).
    {
        CTestAnimFilter f; f.Accept[13] = 1;
        unsigned long seed = 777, want = Next(777);
        const CAnimationEntry& r = Run(f, seed, 12);
        Check(&r == Entry(4) && seed == want, "E single candidate, seed advanced");
    }
    // F: the run ends at Anims.end() (finish after entry 7): no read past it, 7 entries asked.
    {
        Build(banks, 8); g_ents[8 * 5] = 7;   // same key past end(): only the end() test can stop the walk
        CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
        unsigned long seed = 4242, want = Next(4242);
        const CAnimationEntry& r = Run(f, seed, -1);
        Check(f.NSeen == 7 && &r == Entry(1 + want % 7), "F stops at end()");
    }
    // G: FindFirstAnimWithName returns end(): no filter call, end()->second.
    {
        Build(banks, 9); g_first = &g_ents[9 * 5];
        CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
        unsigned long seed = 1;
        const CAnimationEntry& r = Run(f, seed, -1);
        Check(f.NSeen == 0 && (const char*)&r == (const char*)g_set[1] + 4 && seed == 1, "G empty run");
    }
    // H: two accepted entries carry the ignored bank: the later one is remembered and returned.
    {
        static const long dup[9] = { 1, 10, 12, 11, 12, 14, 15, 16, 20 };
        Build(dup, 9);
        CTestAnimFilter f; f.Accept[12] = 1;
        unsigned long seed = 3;
        const CAnimationEntry& r = Run(f, seed, 12);
        Check(&r == Entry(4) && seed == 3, "H last ignored entry wins");
    }
    // I: a key change stops the walk even with more entries after it (start at entry 6: entries 6, 7 only).
    {
        Build(banks, 9); g_first = &g_ents[6 * 5];
        CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
        unsigned long seed = 31337, want = Next(31337);
        const CAnimationEntry& r = Run(f, seed, -1);
        Check(f.NSeen == 2 && &r == Entry(6 + want % 2), "I run from the found entry to the key change");
    }
    // J: the walk ends on ANY key change, a smaller key included (entry 8 rekeyed 2 < 7): 7 entries asked, not 8.
    {
        Build(banks, 9); g_ents[8 * 5] = 2;
        CTestAnimFilter f; for (int i = 0; i < 32; ++i) f.Accept[i] = 1;
        unsigned long seed = 8, want = Next(8);
        const CAnimationEntry& r = Run(f, seed, -1);
        Check(f.NSeen == 7 && &r == Entry(1 + want % 7), "J stops at a smaller key too");
    }
    printf(g_fail ? "005dcc00_TEST FAIL\n" : "005dcc00_TEST PASS\n");
    return g_fail;
}
