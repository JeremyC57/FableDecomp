// Behaviour test for CTCCombat::GetBlockingAnimationEntry (0x00733690): with combat variants a seeded random one is
// returned (seed advanced by the LCG + rotate) and the vector storage freed; without variants GetAnimFromName(name, 0).
#include <cstdio>
#include <cstring>
static int g_frees = 0; static void* g_lastFree = 0; static bool g_armed = false;
static void SpyFree(void* block) { if (g_armed) { ++g_frees; g_lastFree = block; } }   // records only, never frees
static unsigned long GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}
struct CCharString {
    const char* PStringData;
    CCharString(const char* text, long length) : PStringData(text) { }
    ~CCharString() { }
};
struct CAnimationEntry { int id; };
template <class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
    ~CRetailVector() { if (first) SpyFree(first); }
    iterator begin() { return iterator(first); }
    bool empty() const { return first == last; }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
static const CAnimationEntry* g_store[3];
static CAnimationEntry g_a = { 1 }, g_b = { 2 }, g_c = { 3 }, g_plain = { 9 };
struct CThingCreatureBase {
    int Variants; const char* AskedName; long AskedIndex;
    bool GetPossibleCombatAnimations(const CCharString& name, CRetailVector<const CAnimationEntry*>& anims) const
    {
        if (std::strcmp(name.PStringData, "BLOCK_IDLE")) return false;
        g_store[0] = &g_a; g_store[1] = &g_b; g_store[2] = &g_c;
        if (Variants) { anims.first = g_store; anims.last = g_store + Variants; anims.end_of_storage = anims.last; }
        return Variants != 0;
    }
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index) { AskedName = name.PStringData; AskedIndex = index; return &g_plain; }
};
struct CTCCombat {
    CThingCreatureBase* ParentCreature;
};
struct CTCCombat_Methods : CTCCombat {
    unsigned long Seed;
    unsigned long* GetPRandomSeed() { return &Seed; }
    const CAnimationEntry* GetBlockingAnimationEntry();
};

const CAnimationEntry* CTCCombat_Methods::GetBlockingAnimationEntry()
{
    CCharString name("BLOCK_IDLE", -1);
    CRetailVector<const CAnimationEntry*> anims;
    ParentCreature->GetPossibleCombatAnimations(name, anims);
    const CAnimationEntry* entry;
    if (!anims.empty()) {
        unsigned long count = anims.size();
        unsigned long* seed = GetPRandomSeed();
        entry = anims[WorldRandomRange(*seed, count)];
    } else {
        entry = ParentCreature->GetAnimFromName(name, 0);
    }
    return entry;
}

int main()
{
    int bad = 0;
    CThingCreatureBase creature; creature.Variants = 3; creature.AskedName = 0; creature.AskedIndex = -1;
    CTCCombat_Methods combat; combat.ParentCreature = &creature; combat.Seed = 12345;
    unsigned long expect = GFROR13(12345UL * 0x24A1 + 0x24DF);
    g_armed = true;
    const CAnimationEntry* e = combat.GetBlockingAnimationEntry();
    g_armed = false;
    if (combat.Seed != expect || e != g_store[expect % 3] || g_frees != 1 || g_lastFree != g_store || creature.AskedName) bad |= 1;
    creature.Variants = 0; g_armed = true;
    e = combat.GetBlockingAnimationEntry();
    g_armed = false;
    if (e != &g_plain || std::strcmp(creature.AskedName, "BLOCK_IDLE") || creature.AskedIndex != 0 || g_frees != 1 || combat.Seed != expect) bad |= 2;
    if (bad) { std::printf("00733690_TEST FAIL\n"); return 1; }
    std::printf("00733690_TEST PASS\n");
    return 0;
}