#include <cstdio>
// Behaviour test for NCreatureMode::CModeGuildmasterPowerUpHeal::FrameUpdate (retail 0x008DFFD0): the heal timer counts
// down; when it is spent, or the boss (interface 0xad) has no living helpers left, the heal-followers-release action starts.
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
struct CCreatureActionBase { int Kind; };
struct CThingCreatureBase : CThing {
    int actions;
    void SetCurrentAction(const CCreatureActionBase& action) { actions += action.Kind; }
};
struct CCombatAction_Guildmaster_HealFollowersRelease : CCreatureActionBase {
    CCombatAction_Guildmaster_HealFollowersRelease(CThingCreatureBase* owner) { Kind = 1; (void)owner; }
};
struct CTCBoss : CTCBase {
    long Helpers;
    long CountLivingHelperCreatures() const { return Helpers; }
};
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeGuildmasterPowerUpHeal : CModeBase {
    long Timer;
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        long remaining = --Timer;
        CTCBoss* boss = 0;
        ParentThing->GetInterface(&boss, 0xad);
        if (remaining <= 0 || boss->CountLivingHelperCreatures() == 0) {
            CThingCreatureBase* creature = ParentThing;
            creature->SetCurrentAction(CCombatAction_Guildmaster_HealFollowersRelease(creature));
        }
    }
};

int main()
{
    CTCBoss boss;
    boss.Helpers = 2;
    CThingCreatureBase gm;
    gm.ResetInterfaces();
    gm.AddInterface(0xad, &boss);
    gm.actions = 0;
    CModeGuildmasterPowerUpHeal m;
    m.ParentThing = &gm; m.Timer = 3;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.Timer == 2 && gm.actions == 0;
    boss.Helpers = 0;
    m.FrameUpdate();
    pass = pass && gm.actions == 1;
    boss.Helpers = 1;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && gm.actions == 2 && g_baseUpdates == 3;
    if (!pass) { std::printf("008dffd0_TEST FAIL\n"); return 1; }
    std::printf("008dffd0_TEST PASS\n");
    return 0;
}