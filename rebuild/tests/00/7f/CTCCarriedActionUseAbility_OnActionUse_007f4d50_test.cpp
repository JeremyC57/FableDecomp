// Behaviour test for 007f4d50: body copied from src/007f4d50.cpp (CTCCarriedActionUseAbility::OnActionUse), callees stubbed.
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
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Ability;
};
static CAbilityDef* g_ability = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out) const { ++g_getSubDef; if (!g_ability) return false; ++g_ability->RefCount; out->Object = g_ability; return true; }
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x70 - 0x4c];
    const CDefClassBase* PDef;
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
static int g_used = 0; static long g_usedAbility = -1; static bool g_usedFlag = false;
class CTCInventoryAbilities {
public:
    void OnSpecialAbilityUsed(long ability, bool used) { ++g_used; g_usedAbility = ability; g_usedFlag = used; }
};
static int g_showEnded = 0;
class CTCShowableItem {
public:
    void OnShowEnded() { ++g_showEnded; }
};
static int g_baseUse = 0; static CThing* g_baseUser = 0;
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    virtual void OnActionUse(CThing& user) { ++g_baseUse; g_baseUser = &user; }
};
class CTCCarriedActionUseAbility : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};
void CTCCarriedActionUseAbility::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    if (user.HasTC(0x6f)) {
        CTCInventoryAbilities* abilities = user.GetTC<CTCInventoryAbilities>(0x6f);
        CDefPointer<CAbilityDef> out;
        const CDefClassBase* thingDef = ParentThing->PDef;
        bool found = thingDef->GetSubDef(&out);
        CAbilityDef* def = out.Object;
        if (found)
            abilities->OnSpecialAbilityUsed(def->Ability, true);
        if (def) {
            if (--def->RefCount == 0)
                def->Destroy();
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCShowableItem>(0x1a)->OnShowEnded();
}
int main()
{
    static CThing user, item; static CDefClassBase classDef; static CAbilityDef ability; static CTCInventoryAbilities inv; static CTCShowableItem showable;
    memset(&user, 0, sizeof(user)); memset(&item, 0, sizeof(item)); item.PDef = &classDef; ability.RefCount = 1; ability.Ability = 17;
    CInterfaceMap::SEntry userEntries[2] = { { 0x10, 0 }, { 0x6f, (CTCBaseInterface*)&inv } }; user.m_interfaces.first = userEntries; user.m_interfaces.last = userEntries + 2;
    CInterfaceMap::SEntry itemEntries[1] = { { 0x1a, (CTCBaseInterface*)&showable } }; item.m_interfaces.first = itemEntries; item.m_interfaces.last = itemEntries + 1;
    CTCCarriedActionUseAbility a; a.ParentThing = &item;
    // user without an abilities inventory, item without a showable: only the base call
    a.OnActionUse(user); CHECK(g_baseUse == 1 && g_baseUser == &user && g_getSubDef == 0 && g_used == 0 && g_showEnded == 0);
    // user with the inventory, no ability def: nothing used, nothing released
    user.m_presentBits[0x6f >> 5] |= 1UL << (0x6f & 31); g_ability = 0; a.OnActionUse(user); CHECK(g_getSubDef == 1 && g_used == 0 && g_destroyed == 0);
    // ability def found: used, the def released; the showable item notified
    g_ability = &ability; item.m_presentBits[0] |= 1UL << 0x1a; a.OnActionUse(user);
    CHECK(g_used == 1 && g_usedAbility == 17 && g_usedFlag && ability.RefCount == 1 && g_showEnded == 1);
    // a def held only by the lookup is destroyed
    ability.RefCount = 0; a.OnActionUse(user); CHECK(g_destroyed == 1 && g_used == 2 && g_showEnded == 2);
    if (fails == 0) printf("007f4d50_TEST PASS\n"); else printf("007f4d50_TEST FAIL\n");
    return fails;
}