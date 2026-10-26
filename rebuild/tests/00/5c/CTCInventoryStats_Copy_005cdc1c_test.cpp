// behaviour test for 005cdc1c: CTCInventoryStats::Copy -- the base and screen copies run only while the suppress flag is
// clear (the screen base receives the source converted to that base); the two component caches are re-fetched from the
// owner thing either way, and a component the thing does not use leaves its cache untouched
#include <cstdio>
static bool g_SuppressInventoryBaseCopy = false;
static int g_baseCopies = 0, g_screenCopies = 0;
static const void* g_screenSource = 0;
struct CPresenceBits {
    unsigned int bits;
    bool test(long bit) const { return (bits >> bit) & 1u; }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    CPresenceBits UsingComponents;
    CInterfaceMap Components;
    template <class T> void GetInterface(long typeId, T** out) const
    {
        if (UsingComponents.test(typeId)) {
            long type = typeId;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || typeId < it->type) ? map->last : it)->component;
        }
    }
};
struct CScreenBase {
    virtual void Copy(const CScreenBase* other) { ++g_screenCopies; g_screenSource = other; }
};
struct CTCInventoryBase {
    virtual void Copy(const CTCInventoryBase* other) { (void)other; ++g_baseCopies; }
    CThing* ParentThing;
    int pad[8];
};
class CTCInventoryStats : public CTCInventoryBase, public CScreenBase { public:
    void* pTCHeroExperience;
    void* pTCHeroStats;
    virtual void Copy(const CTCInventoryBase* other)
    {
        const CTCInventoryStats* source = static_cast<const CTCInventoryStats*>(other);
        if (!g_SuppressInventoryBaseCopy) {
            CTCInventoryBase::Copy(source);
            CScreenBase::Copy(source);
        }
        ParentThing->GetInterface(0x68 - 0x60, &pTCHeroExperience);   /* 0x68 folded into the 32-bit test mask */
        ParentThing->GetInterface(4, &pTCHeroStats);
    }
};
int main()
{
    int bad = 0;
    int experience = 1, stats = 2, other = 3;
    CInterfaceMap::SEntry entries[3] = { { 2, &other }, { 4, &stats }, { 8, &experience } };
    CThing thing; thing.UsingComponents.bits = (1u << 4) | (1u << 8) | (1u << 2); thing.Components.first = entries; thing.Components.last = entries + 3;
    CTCInventoryStats a, b;
    a.ParentThing = &thing; a.pTCHeroExperience = 0; a.pTCHeroStats = 0;
    b.ParentThing = &thing;
    a.Copy(&b);
    if (g_baseCopies != 1 || g_screenCopies != 1 || g_screenSource != static_cast<const CScreenBase*>(&b)) ++bad;
    if (a.pTCHeroExperience != &experience || a.pTCHeroStats != &stats) ++bad;
    g_SuppressInventoryBaseCopy = true;
    thing.UsingComponents.bits = (1u << 8);        /* hero stats gone: its cache keeps the old pointer */
    a.pTCHeroExperience = 0;
    a.Copy(&b);
    if (g_baseCopies != 1 || g_screenCopies != 1) ++bad;
    if (a.pTCHeroExperience != &experience || a.pTCHeroStats != &stats) ++bad;
    if (bad) { std::printf("005cdc1c_TEST FAIL\n"); return 1; }
    std::printf("005cdc1c_TEST PASS\n");
    return 0;
}