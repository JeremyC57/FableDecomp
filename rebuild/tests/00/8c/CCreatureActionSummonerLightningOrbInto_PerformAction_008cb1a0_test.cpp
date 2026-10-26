// Behaviour test for 0x008CB1A0 CCreatureAction_SummonerLightningOrbInto::PerformAction: runs the reconstructed body over
// stubbed callees; checks the target guards, the mode-manager lookup and the new mode (owner, target) handed to AddMode.
#include <cstdio>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_flags91;
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
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    operator CThing*() const { return GetPItem(); }
};
struct CModeBase { virtual ~CModeBase() { } };
template <class T> class CCountedPointer { public:
    T* m_pObject; void* m_pCount;
    CCountedPointer(T* object) : m_pObject(object), m_pCount(0) { }
};
struct CModeX : CModeBase { CThing* o; CThing* t; CModeX(CThing* owner, CThing* target) : o(owner), t(target) { } };
static CModeX* g_mode = 0; static int g_adds = 0;
class CTCCreatureModeManager { public: bool AddMode(CCountedPointer<CModeBase> mode) { g_mode = (CModeX*)mode.m_pObject; ++g_adds; return true; } };
static int g_base = 0;
class CCombatActionBase {
public:
    virtual void BaseCall() { ++g_base; }
    CBaseIntelligentPointer PCreature; CBaseIntelligentPointer PTarget;
};
class CAction : public CCombatActionBase { public: void Run(); };
void CAction::Run()
{
    CTCCreatureModeManager* modeManager;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeX(PCreature, PTarget.GetPItem())));
}
int main()
{
    CTCCreatureModeManager mm; CInterfaceMap::SEntry e[1] = { { 0x31, (CTCBase*)&mm } };
    CThing owner = CThing(), target = CThing(); owner.m_interfaces.first = e; owner.m_interfaces.last = e + 1;
    CAction a; a.PCreature.m_pItem = &owner; a.PTarget.m_pItem = &target;
    a.Run();                                                        // no mode manager yet
    bool ok = g_adds == 0;
    owner.m_presentBits[1] = 1UL << (0x31 & 31);
    target.m_flags91 = 1; a.Run(); ok = ok && g_adds == 0;          // dead target
    target.m_flags91 = 0; a.Run();
    ok = ok && g_adds == 1 && g_mode && g_mode->o == &owner && g_mode->t == &target;
    delete g_mode;
    if (!ok) { std::printf("008cb1a0_TEST FAIL\n"); return 1; }
    std::printf("008cb1a0_TEST PASS\n");
    return 0;
}