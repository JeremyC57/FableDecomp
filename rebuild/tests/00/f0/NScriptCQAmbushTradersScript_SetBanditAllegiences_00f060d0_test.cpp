// Behaviour test for CQ_AmbushTradersScript::SetBanditAllegiences (0x00F060D0): the old convoy list is emptied (each
// element destroyed), refilled by name, and every living member and the bandit are made enemies both ways; dead
// members are skipped and a terminating thread stops the loop.
#include <cstdio>
#include <cstring>

static int g_live = 0, g_dtors = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* t, long = -1) : PStringData(t) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CBaseClass { virtual ~CBaseClass() { ++g_dtors; } };
struct CScriptThing : CBaseClass { bool alive; virtual bool IsAlive() const { return alive; } };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
static CScriptThing g_members[3];
struct CGameScriptInterface {
    int n; const CScriptThing* a[8]; const CScriptThing* b[8]; const char* name;
    virtual long GetAllThingsWithScriptName(const CCharString& nm, vector<CScriptThing>& v)
    { name = nm.PStringData; v._M_start = g_members; v._M_finish = g_members + 3; v._M_end_of_storage = g_members + 3; return 3; }
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing& t, const CScriptThing& e) { a[n] = &t; b[n] = &e; ++n; }
};
struct CScriptBase { int termAfter; bool IsActiveThreadTerminating() { return termAfter-- == 0; } };
struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    vector<CScriptThing> ConvoyMembersVector;
    void SetBanditAllegiences(const CScriptThing& bandit);
};
void CQ_AmbushTradersScript::SetBanditAllegiences(const CScriptThing& bandit)
{
    while (ConvoyMembersVector.size() != 0)
        ConvoyMembersVector.pop_back();
    m_pInterface->GetAllThingsWithScriptName("AT_ConvoyMember", ConvoyMembersVector);
    for (unsigned int i = 0; i < ConvoyMembersVector.size(); ++i) {
        if (ConvoyMembersVector[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            m_pInterface->EntitySetThingAsEnemyOfThing(bandit, ConvoyMembersVector[i]);
            m_pInterface->EntitySetThingAsEnemyOfThing(ConvoyMembersVector[i], bandit);
        }
    }
}
int main()
{
    int bad = 0;
    static CScriptThing old[2];
    g_members[0].alive = true; g_members[1].alive = false; g_members[2].alive = true;
    CScriptThing bandit; bandit.alive = true;
    CGameScriptInterface gi; gi.n = 0;
    CQ_AmbushTradersScript s; s.m_pInterface = &gi; s.termAfter = 100;
    s.ConvoyMembersVector._M_start = old; s.ConvoyMembersVector._M_finish = old + 2; s.ConvoyMembersVector._M_end_of_storage = old + 2;
    g_dtors = 0;
    s.SetBanditAllegiences(bandit);
    if (g_dtors != 2 || std::strcmp(gi.name, "AT_ConvoyMember") != 0 || gi.n != 4 || g_live != 0) bad = 1;
    if (gi.a[0] != &bandit || gi.b[0] != &g_members[0] || gi.a[1] != &g_members[0] || gi.b[1] != &bandit) bad = 1;
    if (gi.a[2] != &bandit || gi.b[2] != &g_members[2] || gi.a[3] != &g_members[2]) bad = 1;
    // terminate before the first pair
    gi.n = 0; s.termAfter = 0; s.ConvoyMembersVector._M_finish = s.ConvoyMembersVector._M_start;
    s.SetBanditAllegiences(bandit);
    if (gi.n != 0) bad = 1;
    if (bad) { std::printf("00f060d0_TEST FAIL\n"); return 1; }
    std::printf("00f060d0_TEST PASS\n");
    return 0;
}