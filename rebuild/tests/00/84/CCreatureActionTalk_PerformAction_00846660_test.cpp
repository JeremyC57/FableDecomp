// Behaviour test for 0x00846660 CCreatureAction_Talk::PerformAction: runs the reconstructed body over stubbed callees;
// checks the class-byte gate, the entity-events lookup, the CEventTalkedTo(owner) event and the SetHasInformation flags.
#include <cstdio>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
    unsigned char m_thingClass; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
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
static int g_info = 0; static bool g_flags[4];
class CThingAICreature : public CThing { public:
    void SetHasInformation(bool a, bool b, bool c, bool d) { ++g_info; g_flags[0] = a; g_flags[1] = b; g_flags[2] = c; g_flags[3] = d; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CEventBase { virtual ~CEventBase() { } };
struct CEventTalkedTo : CEventBase { CThing* talker; CEventTalkedTo(CThing* t) : talker(t) { } };
static CEventTalkedTo* g_event = 0; static int g_events = 0;
class CTCEntityEvents { public: void AddEvent(CEventBase* e) { g_event = (CEventTalkedTo*)e; ++g_events; } };
class CCreatureActionBase { public: virtual void PerformAction() { } CBaseIntelligentPointer PCreature; };
class CCreatureAction_Talk : public CCreatureActionBase { public: CBaseIntelligentPointer PTarget; virtual void PerformAction(); };
void CCreatureAction_Talk::PerformAction()
{
    if (PTarget.GetPItem() && PTarget.GetPItem()->m_thingClass == 1) {
        CThingAICreature* creature = (CThingAICreature*)PTarget.GetPItem();
        CTCEntityEvents* events;
        if (creature && creature->GetInterface(&events, 0x42)) {
            events->AddEvent(new CEventTalkedTo(PCreature.GetPItem()));
            creature->SetHasInformation(false, false, true, false);
        }
    }
}
int main()
{
    CTCEntityEvents ev; CInterfaceMap::SEntry e[1] = { { 0x42, (CTCBase*)&ev } };
    CThing owner = CThing(); CThingAICreature target; target = CThingAICreature();
    target.m_interfaces.first = e; target.m_interfaces.last = e + 1; target.m_presentBits[2] = 1UL << (0x42 & 31);
    CCreatureAction_Talk a; a.PCreature.m_pItem = &owner; a.PTarget.m_pItem = &target;
    target.m_thingClass = 2; a.PerformAction();                     // not an AI creature
    bool ok = g_events == 0 && g_info == 0;
    target.m_thingClass = 1; a.PerformAction();
    ok = ok && g_events == 1 && g_event && g_event->talker == &owner && g_info == 1 && !g_flags[0] && !g_flags[1] && g_flags[2] && !g_flags[3];
    delete g_event;
    target.m_presentBits[2] = 0; a.PerformAction();                 // no events component
    ok = ok && g_events == 1;
    if (!ok) { std::printf("00846660_TEST FAIL\n"); return 1; }
    std::printf("00846660_TEST PASS\n");
    return 0;
}