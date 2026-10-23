// Behaviour test for CV_MurderTwistScript::Main (0x00EA8830).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    void PostAddScriptedEntities() { Log("gpost"); }
    void CacheMusicSet(long t) { char b[16]; std::sprintf(b, "music%ld", t); Log(b); }
};
struct CV_MurderTwistScript {
    CGameScriptInterface* Game;
    void Init() { Log("init"); }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name, int flags) { Log("bind"); Log(name); Log(flags ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        AddEntityScriptBinding("MurderTwistLeader", 0);
        AddEntityScriptBinding("MurderTwistBrotherYounger", 0);
        AddEntityScriptBinding("MurderTwistBrother", 0);
        AddEntityScriptBinding("MurderTwistBrotherOlder", 0);
        AddEntityScriptBinding("MurderTwistBrotherFriend", 0);
        AddEntityScriptBinding("MurderTwistBrotherOlderFriend", 0);
        AddEntityScriptBinding("MurderTwistBuriedGem", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("MagicRuneProcess");
        SpawnFunction("GemsFoundProcess");
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CV_MurderTwistScript s; s.Game = &game;
    s.Main();
    bool ok = std::strcmp(g_log, "bind;MurderTwistLeader;0;bind;MurderTwistBrotherYounger;0;bind;MurderTwistBrother;0;bind;MurderTwistBrotherOlder;0;bind;MurderTwistBrotherFriend;0;bind;MurderTwistBrotherOlderFriend;0;bind;MurderTwistBuriedGem;0;post;gpost;spawn;MagicRuneProcess;spawn;GemsFoundProcess;") == 0;
    if (!ok) { std::printf("00ea8830_TEST FAIL\n"); return 1; }
    std::printf("00ea8830_TEST PASS\n");
    return 0;
}