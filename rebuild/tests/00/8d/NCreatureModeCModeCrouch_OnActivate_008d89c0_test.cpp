#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeCrouch::OnActivate (retail 0x008D89C0): with a weapon in the primary carry slot
// the crouch filter takes the groups ("CROUCH", <weapon group>); the interface-0x36 component is marked crouching; then the
// base activation runs.
class CTCBase { public: int tag; };
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
class CThing : public CThingLookup { public: };
class CTCWeapon : public CTCBase { public:
    const char* Group;
    const char* GetAnimationGroupName() const { return Group; }
};
class CTCCarrying : public CTCBase { public:
    CThing* Primary;
    CThing* GetThingInPrimarySlot() { return Primary; }
};
struct CTCInterface36 : CTCBase {
    bool m_crouching;
    bool m_flag10;
};
class CAnimFilterOfGroup { public:
    char Group1[16];
    char Group2[16];
    void SetGroups(const char* a, const char* b) { std::strcpy(Group1, a); std::strcpy(Group2, b); }
};
static int g_baseActivations;
struct CModeBase {
    CThing* ParentThing;
    void OnActivate() { ++g_baseActivations; }
};
struct CModeCrouch : CModeBase {
    CAnimFilterOfGroup Filter;
    void OnActivate()
    {
        if (ParentThing->HasTC(0x46)) {
            CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (weapon != 0 && weapon->HasTC(0x48)) {
                CTCWeapon* weaponTC = weapon->GetTC<CTCWeapon>(0x48);
                Filter.SetGroups("CROUCH", weaponTC->GetAnimationGroupName());
            }
        }
        CTCInterface36* tc;
        if (ParentThing->GetInterface(&tc, 0x36))
            tc->m_crouching = true;
        CModeBase::OnActivate();
    }
};

int main()
{
    CTCWeapon weaponTC;
    weaponTC.Group = "BOW";
    CThing bow;
    bow.ResetInterfaces();
    bow.AddInterface(0x48, &weaponTC);
    CTCCarrying carrying;
    carrying.Primary = &bow;
    CTCInterface36 stance;
    stance.m_crouching = false; stance.m_flag10 = false;
    CThing hero;
    hero.ResetInterfaces();
    hero.AddInterface(0x46, &carrying);
    hero.AddInterface(0x36, &stance);
    CModeCrouch m;
    m.ParentThing = &hero;
    m.Filter.Group1[0] = 0; m.Filter.Group2[0] = 0;
    g_baseActivations = 0;
    m.OnActivate();
    int pass = std::strcmp(m.Filter.Group1, "CROUCH") == 0 && std::strcmp(m.Filter.Group2, "BOW") == 0 && stance.m_crouching;
    carrying.Primary = 0;
    m.Filter.Group1[0] = 0;
    stance.m_crouching = false;
    m.OnActivate();
    pass = pass && m.Filter.Group1[0] == 0 && stance.m_crouching && g_baseActivations == 2;
    if (!pass) { std::printf("008d89c0_TEST FAIL\n"); return 1; }
    std::printf("008d89c0_TEST PASS\n");
    return 0;
}