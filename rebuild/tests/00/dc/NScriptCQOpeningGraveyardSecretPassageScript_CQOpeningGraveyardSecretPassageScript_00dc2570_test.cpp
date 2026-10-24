// Behaviour test for 00dc2570: NScript::CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript(void*,
// CGameScriptInterface*) -- the script base is built, the intermediate base stores the game interface and the data, and the
// four hero_object strings are default-constructed; nothing else is initialised.
#include <cstdio>

struct CGameScriptInterface { int dummy; };
static int g_scriptBases = 0;
static int g_strings = 0;
struct CScriptBase {
    virtual ~CScriptBase() { }
    unsigned char _body[0x3c];
    CScriptBase() { ++g_scriptBases; }
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;
    void*                 Data;
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCharString {
    const char* PStringData;
    CCharString() : PStringData("") { ++g_strings; }
};
namespace NScript {
struct CQ_OpeningGraveyardSecretPassageScript : CQuestScriptObject {
    bool finished_quest;
    bool DoorChange;
    bool ZombieCamPlayed;
    bool skeleton_visited;
    bool doors_open;
    bool GravekeeperIntroduced;
    bool GravekeeperCutsceneStart;
    bool UndeadCutscenePlayed;
    bool UndeadCutsceneStarted;
    bool SkeletonStuff[4];
    bool MissionSucceeded;
    bool MissionFailed;
    bool DoorClosed;
    CCharString hero_object[4];
    bool LieutenantAround;
    long FriendliesKilled;
    bool VisitorsAttacked;
    CQ_OpeningGraveyardSecretPassageScript(void* data, CGameScriptInterface* game);
};
CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}

int main()
{
    CGameScriptInterface game;
    static int data;
    NScript::CQ_OpeningGraveyardSecretPassageScript* script = new NScript::CQ_OpeningGraveyardSecretPassageScript(&data, &game);
    if (g_scriptBases != 1 || g_strings != 4 || script->Game != &game || script->Data != &data) {
        std::printf("00dc2570_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 4; ++i)
        if (script->hero_object[i].PStringData[0] != '\0') { std::printf("00dc2570_TEST FAIL\n"); return 1; }
    delete script;
    std::printf("00dc2570_TEST PASS\n");
    return 0;
}