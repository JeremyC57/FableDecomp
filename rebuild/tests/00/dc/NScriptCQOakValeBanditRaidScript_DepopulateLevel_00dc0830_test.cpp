// Behaviour test for CQ_OakValeBanditRaidScript::DepopulateLevel (0x00DC0830): gather every thing scripted
// "OakValeBanditRaidBandit" into a local vector, kill them all without fading, and release the vector.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CScriptThing {
public:
    int id;
};
struct CThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { std::free(_M_start); ++g_frees; }
    static int g_frees;
};
int CThingVector::g_frees = 0;
static int g_killed = 0;
static int g_fadeCalls = 0;
static char g_lastName[64];
class CGameScriptInterface { public:
    long GetAllThingsWithScriptName(const char* name, CThingVector& things) const
    {
        std::strcpy(g_lastName, name);
        things._M_start = (CScriptThing*)std::malloc(3 * sizeof(CScriptThing));
        for (int i = 0; i < 3; ++i) things._M_start[i].id = 100 + i;
        things._M_finish = things._M_start + 3;
        things._M_end_of_storage = things._M_finish;
        return 3;
    }
};
void KillAllThingsInVector(CGameScriptInterface&, const CThingVector& things, bool fade)
{
    for (const CScriptThing* it = things._M_start; it != things._M_finish; ++it) ++g_killed;
    if (fade) ++g_fadeCalls;
}
struct CQ_OakValeBanditRaidScript {
    CGameScriptInterface* Game;
    void DepopulateLevel()
    {
        CThingVector bandits;
        Game->GetAllThingsWithScriptName("OakValeBanditRaidBandit", bandits);
        KillAllThingsInVector(*Game, bandits, false);
    }
};

int main()
{
    CGameScriptInterface game;
    CQ_OakValeBanditRaidScript s;
    s.Game = &game;
    s.DepopulateLevel();
    if (g_killed != 3 || g_fadeCalls != 0 || CThingVector::g_frees != 1 || std::strcmp(g_lastName, "OakValeBanditRaidBandit") != 0) {
        std::printf("00dc0830_TEST FAIL\n");
        return 1;
    }
    std::printf("00dc0830_TEST PASS\n");
    return 0;
}