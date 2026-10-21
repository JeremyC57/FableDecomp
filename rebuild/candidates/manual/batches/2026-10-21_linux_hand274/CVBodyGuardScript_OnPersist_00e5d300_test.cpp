#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CScriptThing {
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CV_BodyGuardScript::OnPersist transfers the four henchman statuses (default 0) and the four respawn
// timers as floats (default 0) under their retail names, in retail order, and writes the transferred times back
struct CPersistContext {
    bool Loading;
    std::string Names;
    void Transfer(const char* name, long& value, const long& defaultValue)
    { Names += name; Names += ";"; if (Loading) value = 100 + (long)Names.size() % 7; (void)defaultValue; }
    void Transfer(const char* name, float& value, const float& defaultValue)
    { Names += name; Names += ";"; if (Loading) value = value * 2.0f + 0.5f; else value = value + defaultValue; }
};
struct CGameScriptInterface {
    long Timers[8];
    long GetTimer(long index) const { return Timers[index]; }
    void SetTimer(long index, long time) { Timers[index] = time; }
};
static CGameScriptInterface g_game;
static CGameScriptInterface* g_pGameScriptInterface = &g_game;
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CV_BodyGuardScript {
    CTimer HenchmanDeadRespawnTimer[4];
    long HenchmanGlobalStatus[4];
    void OnPersist(CPersistContext& context);
};
void CV_BodyGuardScript::OnPersist(CPersistContext& context)
{
    context.Transfer("HenchmanGlobalStatus[HT_EVIL1]", HenchmanGlobalStatus[0], 0);
    context.Transfer("HenchmanGlobalStatus[HT_EVIL2]", HenchmanGlobalStatus[1], 0);
    context.Transfer("HenchmanGlobalStatus[HT_GOOD1]", HenchmanGlobalStatus[2], 0);
    context.Transfer("HenchmanGlobalStatus[HT_GOOD2]", HenchmanGlobalStatus[3], 0);
    float evil1 = (float)HenchmanDeadRespawnTimer[0].GetTime();
    float evil2 = (float)HenchmanDeadRespawnTimer[1].GetTime();
    float good1 = (float)HenchmanDeadRespawnTimer[2].GetTime();
    float good2 = (float)HenchmanDeadRespawnTimer[3].GetTime();
    context.Transfer("Evil1RespawnTimer", evil1, 0.0f);
    context.Transfer("Evil2RespawnTimer", evil2, 0.0f);
    context.Transfer("Good1RespawnTimer", good1, 0.0f);
    context.Transfer("Good2RespawnTimer", good2, 0.0f);
    HenchmanDeadRespawnTimer[0].SetTime((long)evil1);
    HenchmanDeadRespawnTimer[1].SetTime((long)evil2);
    HenchmanDeadRespawnTimer[2].SetTime((long)good1);
    HenchmanDeadRespawnTimer[3].SetTime((long)good2);
}
int main()
{
    CV_BodyGuardScript script;
    for (int i = 0; i < 4; ++i) { script.HenchmanDeadRespawnTimer[i].TimerIndex = 3 + i; script.HenchmanGlobalStatus[i] = 9; g_game.Timers[3 + i] = 10 * (i + 1); }
    CPersistContext save;
    save.Loading = false;
    script.OnPersist(save);
    CHECK("00e5d300", save.Names == "HenchmanGlobalStatus[HT_EVIL1];HenchmanGlobalStatus[HT_EVIL2];HenchmanGlobalStatus[HT_GOOD1];HenchmanGlobalStatus[HT_GOOD2];Evil1RespawnTimer;Evil2RespawnTimer;Good1RespawnTimer;Good2RespawnTimer;");
    CHECK("00e5d300", g_game.Timers[3] == 10 && g_game.Timers[6] == 40 && script.HenchmanGlobalStatus[2] == 9);
    CPersistContext load;
    load.Loading = true;
    script.OnPersist(load);
    CHECK("00e5d300", g_game.Timers[3] == 20 && g_game.Timers[4] == 40 && g_game.Timers[5] == 60 && g_game.Timers[6] == 80);
    CHECK("00e5d300", script.HenchmanGlobalStatus[0] != 9 && script.HenchmanGlobalStatus[3] != 9);
    std::printf("00e5d300_TEST PASS\n");
    return 0;
}
