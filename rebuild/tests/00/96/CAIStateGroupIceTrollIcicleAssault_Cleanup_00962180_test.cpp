// behaviour test for 0x00962180 CAIStateGroup_IceTrollIcicleAssault::Cleanup: a live target with a stealth component is set in danger (DANGER_STATUS_0) from the group's creature; a missing, dead or stealth-less target is left alone
#include <cstdio>
class CThing;
class CTCStealth { public:
    int calls; const CThing* source; int status;
    void SetAsInDanger(const CThing& s, int st) { ++calls; source = &s; status = st; }
};
class CThing { public:
    bool dead;
    CTCStealth* stealth;                    // the component GetInterface(0x36) finds, or 0 when the bit is clear
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (interfaceType == 0x36 && stealth) { *ppOut = (T*)stealth; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->dead)
            return true;
    }
    return false;
}
class CAIBrain { public: CThing* PCreature; };
namespace NStealth { enum EDangerStatus { DANGER_STATUS_0 = 0, DANGER_STATUS_2 = 2, DANGER_STATUS_3 = 3 }; }
struct CGroup {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTarget;
    void Run()
    {
        if (IS_THING_ALIVE(PTarget)) {
            CTCStealth* pStealth;
            if (PTarget.GetPItem()->GetInterface(&pStealth, 0x36))
                pStealth->SetAsInDanger(*PBrain->PCreature, NStealth::DANGER_STATUS_0);
        }
    }
};
int main()
{
    CThing me = { false, 0 };
    CAIBrain brain = { &me };
    CTCStealth st = { 0, 0, -1 };
    CThing target = { false, &st };
    CGroup g; g.PBrain = &brain; g.PTarget.m_pItem = 0;
    g.Run();                                             // no target
    int ok = st.calls == 0;
    g.PTarget.m_pItem = &target; target.dead = true; g.Run();   // dead target
    ok = ok && st.calls == 0;
    target.dead = false; target.stealth = 0; g.Run();    // no stealth component
    ok = ok && st.calls == 0;
    target.stealth = &st; g.Run();
    ok = ok && st.calls == 1 && st.source == &me && st.status == (int)NStealth::DANGER_STATUS_0;
    if (!ok) { std::printf("00962180_TEST FAIL\n"); return 1; }
    std::printf("00962180_TEST PASS\n");
    return 0;
}