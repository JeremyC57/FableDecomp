// Behaviour test for 007edbe0: body copied from src/007edbe0.cpp (CTCCarriedActionUseFish::OnActionUse), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CBonusItemDef : CDefPointeeBase {
};
static CBonusItemDef* g_bonus = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CBonusItemDef>* out) const { ++g_getSubDef; if (!g_bonus) return false; ++g_bonus->RefCount; out->Object = g_bonus; return true; }
};
static int g_kills = 0; static class CThing* g_killed = 0;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x70 - 0x4c];
    const CDefClassBase* PDef;
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately) { ++g_kills; g_killed = this; CHECK(immediately); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CCreatureActionBase;
static int g_setAction = 0; static const CCreatureActionBase* g_action = 0;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action) { ++g_setAction; g_action = &action; return true; }
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }
static int g_applied = 0; static const CBonusItemDef* g_appliedDef = 0;
class CTCHeroStats {
public:
    void ApplyBonusItemEffects(const CBonusItemDef& def) { ++g_applied; g_appliedDef = &def; }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer() { m_pItem = 0; }
};
static int g_actionDtors = 0;
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() { ++g_actionDtors; }
    unsigned char _pad04[0xa8 - 4];
};
static int g_wallMountCtors = 0; static CThingCreatureBase* g_wmCreature = 0; static CThing* g_wmFish = 0;
class CCreatureAction_UseWallMount : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PWallMount;
    CCreatureAction_UseWallMount(CThingCreatureBase& creature, CThing& fish) { ++g_wallMountCtors; g_wmCreature = &creature; g_wmFish = &fish; PWallMount.m_pItem = &fish; }
};
static int g_baseUse = 0; static bool g_wallTargeted = false;
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase() {}
    CThing* ParentThing;
    CThing& GetParentThing() const { return *ParentThing; }
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    virtual void OnActionUse(CThing& user) { ++g_baseUse; }
};
class CTCCarriedActionUseFish : public CTCActionUseBase {
public:
    bool IsWallMountTargeted(const CThing& user) const { return g_wallTargeted; }
    virtual void OnActionUse(CThing& user);
};
void CTCCarriedActionUseFish::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    if (IsWallMountTargeted(user)) {
        CThingCreatureBase* creature = CastToCreature(&user);
        if (creature && !creature->IsDead())
            creature->SetCurrentAction(CCreatureAction_UseWallMount(*creature, GetParentThing()));
        return;
    }
    CDefPointer<CBonusItemDef> out;
    const CDefClassBase* thingDef = ParentThing->PDef;
    bool found = thingDef->GetSubDef(&out);
    CBonusItemDef* def = out.Object;
    CTCHeroStats* stats;
    if (found && user.GetInterface(&stats, 4))
        stats->ApplyBonusItemEffects(*def);
    ParentThing->Kill(true);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}
int main()
{
    static CThingCreatureBase user; static CThing fish; static CDefClassBase classDef; static CBonusItemDef bonus; static CTCHeroStats stats;
    memset(&user, 0, sizeof(user)); memset(&fish, 0, sizeof(fish)); fish.PDef = &classDef; bonus.RefCount = 1;
    CInterfaceMap::SEntry entries[2] = { { 2, 0 }, { 4, (CTCBaseInterface*)&stats } }; user.m_interfaces.first = entries; user.m_interfaces.last = entries + 2;
    CTCCarriedActionUseFish a; a.ParentThing = &fish;
    // wall mount targeted, live creature: the use-wall-mount action is set on the creature with this fish; the fish is not eaten
    g_wallTargeted = true; a.OnActionUse(user);
    CHECK(g_baseUse == 1 && g_setAction == 1 && g_wallMountCtors == 1 && g_wmCreature == &user && g_wmFish == &fish && g_actionDtors == 1); CHECK(g_getSubDef == 0 && g_kills == 0);
    // wall mount targeted but the creature is dead: nothing
    user.Flags91 = 1; a.OnActionUse(user); CHECK(g_setAction == 1 && g_kills == 0); user.Flags91 = 0;
    // eaten: no bonus def, user without hero stats -> only killed
    g_wallTargeted = false; g_bonus = 0; a.OnActionUse(user); CHECK(g_getSubDef == 1 && g_applied == 0 && g_kills == 1 && g_killed == &fish && g_destroyed == 0);
    // bonus def but no hero stats: killed, def released
    g_bonus = &bonus; a.OnActionUse(user); CHECK(g_applied == 0 && g_kills == 2 && bonus.RefCount == 1);
    // bonus def and hero stats: effects applied, killed, def released
    user.m_presentBits[0] |= 1UL << 4; a.OnActionUse(user); CHECK(g_applied == 1 && g_appliedDef == &bonus && g_kills == 3 && bonus.RefCount == 1 && g_destroyed == 0);
    // a def held only by the lookup is destroyed
    bonus.RefCount = 0; a.OnActionUse(user); CHECK(g_destroyed == 1 && g_applied == 2);
    if (fails == 0) printf("007edbe0_TEST PASS\n"); else printf("007edbe0_TEST FAIL\n");
    return fails;
}