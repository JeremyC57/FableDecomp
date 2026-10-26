// Behaviour test for CQ_ArenaScript::InitialiseVariables (0x00F25840): the rounds vector is copied from the
// script def and the 4x5 crowd-loop tag table gets the retail sound tags.
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* PStringData;
    CCharString& operator=(const char* text) { PStringData = text; return *this; }
};
class CArenaRoundDef { public: int v; };
struct CArenaRoundVector {
    CArenaRoundDef* _M_start; CArenaRoundDef* _M_finish; CArenaRoundDef* _M_end_of_storage;
    int copies;
    CArenaRoundVector& operator=(const CArenaRoundVector& o)
    { _M_start = o._M_start; _M_finish = o._M_finish; _M_end_of_storage = o._M_end_of_storage; ++copies; return *this; }
};
class CScriptDef { public: CArenaRoundVector ArenaRounds; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;

struct CQ_ArenaScript {
    CCharString CrowdLoopTags[4][5];
    CArenaRoundVector Rounds;
    void InitialiseVariables();
};

void CQ_ArenaScript::InitialiseVariables()
{
    Rounds = g_pScriptDef->ArenaRounds;
    CrowdLoopTags[0][0] = "ARENA_HECKLE_SMALL_LOOP";
    CrowdLoopTags[0][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[0][2] = "ARENA_APPLAUSE_SMALL_LOOP";
    CrowdLoopTags[0][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[0][4] = "ARENA_AWWW";
    CrowdLoopTags[1][0] = "ARENA_HECKLE_SMALL_LOOP";
    CrowdLoopTags[1][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[1][2] = "ARENA_APPLAUSE_MEDIUM_LOOP";
    CrowdLoopTags[1][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[1][4] = "ARENA_AWWW";
    CrowdLoopTags[2][0] = "ARENA_HECKLE_MEDIUM_LOOP";
    CrowdLoopTags[2][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[2][2] = "ARENA_APPLAUSE_BIG_LOOP";
    CrowdLoopTags[2][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[2][4] = "ARENA_AWWW";
    CrowdLoopTags[3][0] = "ARENA_HECKLE_BIG_LOOP";
    CrowdLoopTags[3][1] = "ARENA_CHANT_LOOP";
    CrowdLoopTags[3][2] = "ARENA_APPLAUSE_BIG_LOOP";
    CrowdLoopTags[3][3] = "ARENA_CHEER_CROWD";
    CrowdLoopTags[3][4] = "ARENA_AWWW";
}

int main()
{
    static CArenaRoundDef defs[3];
    g_def.ArenaRounds._M_start = defs; g_def.ArenaRounds._M_finish = defs + 2; g_def.ArenaRounds._M_end_of_storage = defs + 3;
    CQ_ArenaScript s;
    std::memset(&s, 0, sizeof(s));
    s.InitialiseVariables();
    int bad = 0;
    if (s.Rounds._M_start != defs || s.Rounds._M_finish != defs + 2 || s.Rounds._M_end_of_storage != defs + 3 || s.Rounds.copies != 1) bad = 1;
    if (std::strcmp(s.CrowdLoopTags[0][2].PStringData, "ARENA_APPLAUSE_SMALL_LOOP") != 0) bad = 1;
    if (std::strcmp(s.CrowdLoopTags[1][2].PStringData, "ARENA_APPLAUSE_MEDIUM_LOOP") != 0) bad = 1;
    if (std::strcmp(s.CrowdLoopTags[2][0].PStringData, "ARENA_HECKLE_MEDIUM_LOOP") != 0) bad = 1;
    if (std::strcmp(s.CrowdLoopTags[3][0].PStringData, "ARENA_HECKLE_BIG_LOOP") != 0) bad = 1;
    for (int i = 0; i < 4; ++i) {
        if (std::strcmp(s.CrowdLoopTags[i][1].PStringData, "ARENA_CHANT_LOOP") != 0) bad = 1;
        if (std::strcmp(s.CrowdLoopTags[i][3].PStringData, "ARENA_CHEER_CROWD") != 0) bad = 1;
        if (std::strcmp(s.CrowdLoopTags[i][4].PStringData, "ARENA_AWWW") != 0) bad = 1;
    }
    if (bad) { std::printf("00f25840_TEST FAIL\n"); return 1; }
    std::printf("00f25840_TEST PASS\n");
    return 0;
}