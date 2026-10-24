#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeCarryingWeaponStand::OnActivate (retail 0x008D87C0): the stand filter's group is
// cleared, then set to the animation group of the weapon in the primary carry slot when that thing is a weapon
// (interfaces 0x46 carrying / 0x48 weapon), then the carrying-stand activation runs.
struct CTCBase { int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThingLookup {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_store[9];
    void ResetInterfaces()
    {
        unsigned int w = 0;
        while (w < 9) { m_presentBits[w] = 0; m_store[w].type = 0x7fff; m_store[w].component = 0; ++w; }
        m_interfaces.first = m_store;
        m_interfaces.last = m_store;
    }
    void AddInterface(long type, CTCBase* component)
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
struct CThing : CThingLookup { };
struct CTCWeapon : CTCBase {
    const char* Group;
    const char* GetAnimationGroupName() const { return Group; }
};
struct CTCCarrying : CTCBase {
    CThing* Primary;
    CThing* GetThingInPrimarySlot() { return Primary; }
};
struct CAnimFilterOfGroup {
    char Group[32];
    void SetGroupInline(const char* name) { std::strcpy(Group, name); }
    void SetGroup(const char* name) { std::strcpy(Group, name); }
};
static int g_standActivations;
struct CModeCarryingStandBase {
    CThing* ParentThing;
    CAnimFilterOfGroup Filter;
    void OnActivate() { ++g_standActivations; }
};
struct CModeCarryingWeaponStand : CModeCarryingStandBase {
    void OnActivate()
    {
        Filter.SetGroupInline("");
        if (ParentThing->HasTC(0x46)) {
            CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (weapon != 0 && weapon->HasTC(0x48))
                Filter.SetGroup(weapon->GetTC<CTCWeapon>(0x48)->GetAnimationGroupName());
        }
        CModeCarryingStandBase::OnActivate();
    }
};

int main()
{
    CTCWeapon weaponTC;
    weaponTC.Group = "SWORD";
    CThing sword;
    sword.ResetInterfaces();
    sword.AddInterface(0x48, &weaponTC);
    CThing plate;
    plate.ResetInterfaces();
    CTCCarrying carrying;
    carrying.Primary = &sword;
    CThing hero;
    hero.ResetInterfaces();
    hero.AddInterface(0x30, 0);
    hero.AddInterface(0x46, &carrying);
    CModeCarryingWeaponStand m;
    m.ParentThing = &hero;
    std::strcpy(m.Filter.Group, "OLD");
    g_standActivations = 0;
    m.OnActivate();
    int pass = std::strcmp(m.Filter.Group, "SWORD") == 0 && g_standActivations == 1;
    carrying.Primary = &plate;
    m.OnActivate();
    pass = pass && m.Filter.Group[0] == 0;
    std::strcpy(m.Filter.Group, "OLD");
    carrying.Primary = 0;
    m.OnActivate();
    pass = pass && m.Filter.Group[0] == 0 && g_standActivations == 3;
    if (!pass) { std::printf("008d87c0_TEST FAIL\n"); return 1; }
    std::printf("008d87c0_TEST PASS\n");
    return 0;
}