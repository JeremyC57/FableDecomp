#include <cstdio>
// Behaviour test for NCreatureMode::CModeBlock::FrameUpdate (retail 0x008D8EA0): a blocking creature with a look component
// (interface 0x43) that is a registered combatant looks (priority 0x21) at the live creature it is attacking; then the
// base update.
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
class C3DVector { public: float X, Y, Z; };
class CThing : public CThingLookup { public:
    unsigned char m_flags91;
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};
class CThingCreatureBase : public CThing { public:
    bool Registered;
    bool IsRegisteredCombatant() const { return Registered; }
};
class CTCLook : public CTCBase { public:
    const C3DVector* At;
    long Priority;
    void LookAtPos(const C3DVector* position, long priority) { At = position; Priority = priority; }
};
class CAttackingCombatantInfo { public: CThingCreatureBase* Attacked; };
struct CCombatantInfo { CThingCreatureBase* Parent; CAttackingCombatantInfo AttackerInfo; };
class CCombatManager { public:
    CCombatantInfo Info;
    CCombatantInfo* GetCombatantInfo(const CThingCreatureBase* thing) { Info.Parent = (CThingCreatureBase*)thing; return &Info; }
};
class CWorld { public: CCombatManager* CombatManager; };
static CWorld g_world;
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    CWorld* GetWorld() const { return &g_world; }
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeBlock : CModeBase {
    CThingCreatureBase* PCreature;
    void FrameUpdate()
    {
        CTCLook* look;
        if (ParentThing->GetInterface(&look, 0x43)) {
            if (PCreature->IsRegisteredCombatant()) {
                CCombatManager* combatManager = GetWorld()->CombatManager;
                CCombatantInfo* info = combatManager->GetCombatantInfo(PCreature);
                if (info->AttackerInfo.Attacked != 0 && !(info->AttackerInfo.Attacked->m_flags91 & 1))
                    look->LookAtPos(&info->AttackerInfo.Attacked->GetPosition(), 0x21);
            }
        }
        CModeBase::FrameUpdate();
    }
};

int main()
{
    CCombatManager manager;
    g_world.CombatManager = &manager;
    CTCLook look;
    look.At = 0; look.Priority = 0;
    CThingCreatureBase guard;
    guard.ResetInterfaces();
    guard.AddInterface(0x43, &look);
    guard.Registered = true; guard.m_flags91 = 0;
    CThingCreatureBase enemy;
    enemy.ResetInterfaces();
    enemy.m_flags91 = 0;
    manager.Info.AttackerInfo.Attacked = &enemy;
    CModeBlock m;
    m.ParentThing = &guard; m.PCreature = &guard;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = look.At == &enemy.Position && look.Priority == 0x21 && manager.Info.Parent == &guard;
    look.At = 0;
    enemy.m_flags91 = 1;
    m.FrameUpdate();
    pass = pass && look.At == 0;
    enemy.m_flags91 = 0; guard.Registered = false;
    m.FrameUpdate();
    pass = pass && look.At == 0;
    guard.Registered = true; manager.Info.AttackerInfo.Attacked = 0;
    m.FrameUpdate();
    pass = pass && look.At == 0 && g_baseUpdates == 4;
    if (!pass) { std::printf("008d8ea0_TEST FAIL\n"); return 1; }
    std::printf("008d8ea0_TEST PASS\n");
    return 0;
}