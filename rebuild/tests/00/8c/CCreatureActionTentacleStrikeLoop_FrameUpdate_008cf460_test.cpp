// Behaviour test for 0x008CF460 CCreatureAction_TentacleStrikeLoop::FrameUpdate: runs the reconstructed body over stubbed callees;
// checks the health-drop and kraken-hit interrupts (Interrupt + virtual FinishAction) and the base FrameUpdate.

#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_flags91; float m_health;
    CInterfaceMap::SEntry m_entries[4]; int m_count;
    CThing() : m_flags91(0), m_health(0), m_count(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void Add(long type, void* component)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_entries[m_count].type = type; m_entries[m_count].component = (CTCBase*)component; ++m_count;
        m_interfaces.first = m_entries; m_interfaces.last = m_entries + m_count;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static inline bool IsThingAlive(const CThing* thing) { return thing && !(thing->m_flags91 & 1); }
static bool g_hit = false;
class CTCKraken { public: bool HasTargetBeenHitByKraken() { return g_hit; } };
class CTCKrakenTentacle { public: CBaseIntelligentPointer PKraken; };
static int g_finish = 0, g_frame = 0;
class CCombatActionBase { public:
    virtual void FinishAction() { ++g_finish; }
    virtual void FrameUpdate() { ++g_frame; }
    CBaseIntelligentPointer PCreature;
    CTCKrakenTentacle* GetTentacle() const { CTCKrakenTentacle* t = 0; PCreature.GetPItem()->GetInterface(&t, 0xe3); return t; }
};
class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase { public:
    long StartTick; bool Interrupt; float StartHealth;
    virtual void FrameUpdate();
};
void CCreatureAction_TentacleStrikeLoop::FrameUpdate()
{
    CTCKrakenTentacle* tentacle = GetTentacle();
    CTCKraken* kraken;
    if (StartHealth > PCreature.GetPItem()->m_health ||
        (IsThingAlive(tentacle->PKraken.GetPItem()) &&
         tentacle->PKraken.GetPItem()->GetInterface(&kraken, 0xe2) && kraken->HasTargetBeenHitByKraken())) {
        Interrupt = true;
        FinishAction();
    }
    CCombatActionBase::FrameUpdate();
}
int main()
{
    CTCKraken k; CThing krakenThing = CThing(); krakenThing.Add(0xe2, &k);
    CTCKrakenTentacle t; t.PKraken.m_pItem = &krakenThing;
    CThing owner = CThing(); owner.Add(0xe3, &t); owner.m_health = 100;
    CCreatureAction_TentacleStrikeLoop a; a.PCreature.m_pItem = &owner; a.StartHealth = 100; a.Interrupt = false;
    a.FrameUpdate();                                   // equal health, no hit
    bool ok = !a.Interrupt && g_finish == 0 && g_frame == 1;
    g_hit = true; a.FrameUpdate();                     // kraken hit
    ok = ok && a.Interrupt && g_finish == 1 && g_frame == 2;
    a.Interrupt = false; krakenThing.m_flags91 = 1; a.FrameUpdate();   // dead kraken: hit ignored
    ok = ok && !a.Interrupt && g_finish == 1;
    owner.m_health = 50; a.FrameUpdate();              // health dropped
    ok = ok && a.Interrupt && g_finish == 2 && g_frame == 4;
    if (!ok) { std::printf("008cf460_TEST FAIL\n"); return 1; }
    std::printf("008cf460_TEST PASS\n");
    return 0;
}