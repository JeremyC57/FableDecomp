// Behaviour test for 0x00E01670 CDarkwoodTrader constructor: Game comes from the parent's interface (+0x40 in retail), Me is
// copied, ParentClass/PMasterData follow the arguments and each of the three timers gets its own timer index.
#include <cstdio>
static int g_nextTimer = 100;
struct CTimer { int TimerIndex; CTimer() : TimerIndex(g_nextTimer++) {} };
struct CScriptThing { int m_id; };
struct CScriptOwner { void* m_pInterface; };
struct CEntityScriptBase {
    void* Game; CScriptThing Me;
    CEntityScriptBase(const CScriptThing& t, void* g) : Game(g), Me(t) {}
};
struct CDarkwoodTrader : CEntityScriptBase {
    CScriptOwner* ParentClass; void* PMasterData;
    CTimer IncubationTime; CTimer RegulateFollowStateComment; CTimer RegulateBanterComment;
    CDarkwoodTrader(CScriptOwner* parent, void* masterData, const CScriptThing& me)
        : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), IncubationTime(), RegulateFollowStateComment(), RegulateBanterComment()
    { PMasterData = masterData; }
};
int main()
{
    int iface = 0, master = 0;
    CScriptOwner owner; owner.m_pInterface = &iface;
    CScriptThing me; me.m_id = 42;
    CDarkwoodTrader t(&owner, &master, me);
    if (t.Game != &iface || t.Me.m_id != 42 || t.ParentClass != &owner || t.PMasterData != &master
        || t.IncubationTime.TimerIndex != 100 || t.RegulateFollowStateComment.TimerIndex != 101 || t.RegulateBanterComment.TimerIndex != 102)
        { std::printf("00e01670_TEST FAIL\n"); return 1; }
    std::printf("00e01670_TEST PASS\n");
    return 0;
}