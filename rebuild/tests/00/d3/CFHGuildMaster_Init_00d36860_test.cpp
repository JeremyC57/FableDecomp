// Behaviour test for 0x00d36860 CFH_GuildMaster::Init
#include <cstdio>
struct SCount { long m_references; };
class CScriptThing { public:
    void* m_pThing; SCount* m_pCount;
    CScriptThing(void* t, SCount* c) : m_pThing(t), m_pCount(c) {}
    CScriptThing(const CScriptThing& o) : m_pThing(o.m_pThing), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CScriptThing() { if (m_pCount) --m_pCount->m_references; }
};
class CGame { public:
    int log[8]; int n; long refsSeen;
    void EntitySetAsKillable(const CScriptThing&, bool killable, bool b) { log[n++] = 517 + killable * 1000 + b * 2000; }
    void SetIsPushableByHero(CScriptThing thing, bool pushable) { log[n++] = 844 + pushable * 1000; refsSeen = thing.m_pCount->m_references; }
    void SetIsThingForcePushable(const CScriptThing&, bool pushable) { log[n++] = 745 + pushable * 1000; }
    void EntitySetAsUseMovementInActions(const CScriptThing&, bool use) { log[n++] = 529 + use * 1000; }
};
struct CFH_GuildMaster {
    CGame* Game; CScriptThing Me; long NextLine;
    CFH_GuildMaster(CGame* g, SCount* c) : Game(g), Me(0, c), NextLine(0) {}
    void Init()
    {
        Game->EntitySetAsKillable(Me, false, true);
        Game->SetIsPushableByHero(Me, false);
        Game->SetIsThingForcePushable(Me, false);
        Game->EntitySetAsUseMovementInActions(Me, false);
        NextLine = 1;
    }
};
int main()
{
    SCount c = { 1 }; CGame g; g.n = 0; g.refsSeen = 0;
    CFH_GuildMaster m(&g, &c); m.Init();
    bool ok = g.n == 4 && g.log[0] == 2517 && g.log[1] == 844 && g.log[2] == 745 && g.log[3] == 529 && m.NextLine == 1 && g.refsSeen == 2 && c.m_references == 1;
    if (!ok) { std::printf("00d36860_TEST FAIL\n"); return 1; }
    std::printf("00d36860_TEST PASS\n");
    return 0;
}