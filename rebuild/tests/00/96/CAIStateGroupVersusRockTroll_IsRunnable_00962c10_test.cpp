// Behaviour test for 00962c10: body copied from src/00962c10.cpp (CAIStateGroup_VersusRockTroll::IsRunnable), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;
    unsigned char _pad2[0x70 - 0x64];
    void* PDef;
    unsigned char _pad3[0x91 - 0x74];
    unsigned char m_flags91;
    unsigned char m_flags92;
    template<class T> bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
static int g_setPItem = 0; static CThing* g_lastSet = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    void SetPItem(CThing* item) { ++g_setPItem; g_lastSet = item; m_pItem = item; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
static unsigned long g_ror13Calls = 0;
unsigned long __fastcall GFROR13(unsigned long value) { ++g_ror13Calls; return (value >> 13) | (value << 19); }
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
static unsigned long g_seed = 0;
struct CWorldR {
    unsigned long* GetPRandomSeed() { return &g_seed; }
};
static CWorldR g_worldR;
static CThing* g_enemy = 0; static int g_getEnemy = 0;
struct CTCEnemyTracker {
    virtual void v00() {}
    CThing* GetEnemy() { ++g_getEnemy; return g_enemy; }
};
static int g_destroyed = 0;
struct CAbilityDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    unsigned char _pad08[0xac - 0x08];
    unsigned long AbilityType;
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
static CAbilityDef* g_ability = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef_00485815(CDefPointer<CAbilityDef>* out)
    {
        ++g_getSubDef;
        if (!g_ability) return false;
        ++g_ability->m_refCount; out->Object = g_ability; return true;
    }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointerCreature() : CBaseIntelligentPointer() {}
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
inline CDefClassBase* GetThingDef(CThing* thing) { return *(CDefClassBase**)((unsigned char*)thing + 0x70); }
struct CAIStateGroup_VersusRockTroll : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;
    bool UseRockThrowing;
    CWorldR* GetPWorld() { return &g_worldR; }
    virtual bool IsRunnable();
};

bool CAIStateGroup_VersusRockTroll::IsRunnable()
{
    CTCEnemyTracker* tracker;
    PBrain->PCreature->GetInterfaceX(&tracker, 8);
    CThing* enemy = tracker->GetEnemy();
    CDefPointer<CAbilityDef> holder;
    if (enemy && !(enemy->m_flags91 & 1)) {
        bool found = GetThingDef(enemy)->GetSubDef_00485815(&holder);
        CAbilityDef* ability = holder.Object;
        if (found && (ability->AbilityType == 0x10000 || ability->AbilityType == 0x8000)) {
            unsigned long* seed = GetPWorld()->GetPRandomSeed();
            unsigned long next = *seed * 0x24A1U + 0x24DFU;
            *seed = next;
            unsigned long rotated = GFROR13(next);
            *seed = rotated;
            UseRockThrowing = rotated % 100 > 50;
            PEnemy = CIntelligentPointerCreature(enemy);
            ability->Release();
            return true;
        }
        if (ability)
            ability->Release();
    }
    return false;
}
static unsigned long Expect(unsigned long seed) { unsigned long n = seed * 0x24A1U + 0x24DFU; return (n >> 13) | (n << 19); }
int main()
{
    static CThing creature, enemy; static CAIBrain brain; static CTCEnemyTracker tracker; static CDefClassBase classDef; static CAbilityDef ability;
    memset(&creature, 0, sizeof(creature)); memset(&enemy, 0, sizeof(enemy));
    CInterfaceMap::SEntry entries[2] = { { 2, 0 }, { 8, (CTCBase*)&tracker } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2; creature.m_presentBits[0] |= 1UL << 8;
    enemy.PDef = &classDef; brain.PCreature = &creature; ability.m_refCount = 1; g_ability = &ability;
    CAIStateGroup_VersusRockTroll g; g.PBrain = &brain; g.UseRockThrowing = false;
    // no enemy: not runnable, no lookup
    g_enemy = 0; CHECK(!g.IsRunnable()); CHECK(g_getEnemy == 1 && g_getSubDef == 0);
    // dead enemy: not runnable
    g_enemy = &enemy; enemy.m_flags91 = 1; CHECK(!g.IsRunnable()); CHECK(g_getSubDef == 0);
    // alive enemy without an ability def: not runnable, nothing released
    enemy.m_flags91 = 0; g_ability = 0; CHECK(!g.IsRunnable()); CHECK(g_getSubDef == 1 && g_destroyed == 0 && g_setPItem == 0);
    // alive enemy with another ability type: not runnable, the def released
    g_ability = &ability; ability.AbilityType = 0x4000; CHECK(!g.IsRunnable()); CHECK(ability.m_refCount == 1 && g_setPItem == 0);
    // rock troll ability 0x10000: runnable, the seed stepped, the enemy remembered, rock throwing from the draw
    ability.AbilityType = 0x10000; g_seed = 5; unsigned long expected = Expect(5);
    CHECK(g.IsRunnable()); CHECK(g_seed == expected && g_ror13Calls == 1); CHECK(g.UseRockThrowing == (expected % 100 > 50)); CHECK(g_lastSet == &enemy && g.PEnemy.m_pItem == &enemy); CHECK(ability.m_refCount == 1 && g_destroyed == 0);
    // ability 0x8000 also qualifies; find a seed giving the other outcome
    ability.AbilityType = 0x8000; unsigned long s = 0; while ((Expect(s) % 100 > 50) == (expected % 100 > 50)) ++s;
    g_seed = s; CHECK(g.IsRunnable()); CHECK(g.UseRockThrowing == (Expect(s) % 100 > 50) && g.UseRockThrowing != (expected % 100 > 50));
    // a def held only by the lookup is destroyed on the runnable path
    ability.m_refCount = 0; CHECK(g.IsRunnable()); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00962c10_TEST PASS\n"); else printf("00962c10_TEST FAIL\n");
    return fails;
}