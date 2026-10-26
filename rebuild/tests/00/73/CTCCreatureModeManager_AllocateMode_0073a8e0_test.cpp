// Behaviour test for CTCCreatureModeManager::AllocateMode (0x0073A8E0): with N modes registered for the type the seeded
// random index picks one allocator, which builds the mode for the parent thing; the result holds a fresh count block
// (1 reference, the deleter, the mode); a null mode gives an empty pointer; no registration -> index 0 of the range.
#include <cstdio>
class CThing { public: int id; };
struct CModeBase { CThing* Owner; int Kind; };
static void Destroy_007180b0(void* object) { }
static unsigned long GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};
static CModeBase g_modes[3];
static CModeBase* MakeA(CThing* t) { g_modes[0].Owner = t; g_modes[0].Kind = 1; return &g_modes[0]; }
static CModeBase* MakeB(CThing* t) { g_modes[1].Owner = t; g_modes[1].Kind = 2; return &g_modes[1]; }
static CModeBase* MakeNull(CThing* t) { return 0; }
struct SRegisteredMode {
    unsigned long first;
    CModeBase* (*Allocate)(CThing* thing);
};
static SRegisteredMode g_entries[4] = { { 3, MakeNull }, { 7, MakeA }, { 7, MakeB }, { 9, MakeA } };
struct CModeRegistry {
    const SRegisteredMode* UpperBound(const unsigned long& key) const { const SRegisteredMode* p = g_entries; while (p != g_entries + 4 && p->first <= key) ++p; return p; }
    const SRegisteredMode* LowerBound(const unsigned long& key) const { const SRegisteredMode* p = g_entries; while (p != g_entries + 4 && p->first < key) ++p; return p; }
    SRegisteredMode* LowerBound(const unsigned long& key) { SRegisteredMode* p = g_entries; while (p != g_entries + 4 && p->first < key) ++p; return p; }
};
static CModeRegistry g_CreatureModeRegistry;
static unsigned long g_seed = 77;
class CWorld { public: unsigned long* GetPRandomSeed() { return &g_seed; } };
static CWorld g_world;
inline long CountRegisteredModes(const CModeRegistry& registry, const unsigned long& type)
{
    const SRegisteredMode* last = registry.UpperBound(type);
    return last - registry.LowerBound(type);
}
class CTCCreatureModeManager { public:
    virtual void v00() { }
    CThing* ParentThing;
    CWorld* GetWorld() const { return &g_world; }
    CCountedPointer<CModeBase> AllocateMode(unsigned long modeType);
};

CCountedPointer<CModeBase> CTCCreatureModeManager::AllocateMode(unsigned long modeType)
{
    long count = CountRegisteredModes(g_CreatureModeRegistry, modeType);
    unsigned long* seed = GetWorld()->GetPRandomSeed();
    unsigned long index = WorldRandomRange(*seed, count);
    CThing* thing = ParentThing;
    return CCountedPointer<CModeBase>(g_CreatureModeRegistry.LowerBound(modeType)[index].Allocate(thing));
}

int main()
{
    int bad = 0;
    CThing owner; owner.id = 5;
    CTCCreatureModeManager m; m.ParentThing = &owner;
    unsigned long s = 77; unsigned long expect = WorldRandomRange(s, 2);
    CCountedPointer<CModeBase> p = m.AllocateMode(7);
    if (g_seed != s || p.m_pObject != &g_modes[expect] || p.m_pObject->Owner != &owner || !p.m_pCount
        || p.m_pCount->m_references != 1 || p.m_pCount->m_pObject != p.m_pObject || p.m_pCount->m_pfnDestroy != Destroy_007180b0) bad |= 1;
    delete p.m_pCount;
    CCountedPointer<CModeBase> q = m.AllocateMode(3);   // the allocator returns null: empty pointer
    if (q.m_pObject || q.m_pCount) bad |= 2;
    CCountedPointer<CModeBase> r = m.AllocateMode(9);   // one registration: index 0
    if (r.m_pObject != &g_modes[0] || g_modes[0].Kind != 1) bad |= 4;
    delete r.m_pCount;
    if (bad) { std::printf("0073a8e0_TEST FAIL\n"); return 1; }
    std::printf("0073a8e0_TEST PASS\n");
    return 0;
}