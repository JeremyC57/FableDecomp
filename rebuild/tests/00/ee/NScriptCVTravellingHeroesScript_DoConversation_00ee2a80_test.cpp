// Behaviour test for CV_TravellingHeroesScript::DoConversation (0x00EE2A80).
#include <cstdio>
#include <cstring>
#include <string>
static char g_log[2048];
static void Log(const std::string& s) { std::strcat(g_log, s.c_str()); std::strcat(g_log, ";"); }
class CScriptThing { public: std::string name; };
struct Handle {
    std::string who;
    void PlayAnimation(const std::string& a, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7)
    { Log(!b1 && b2 && !b3 && !b4 && !b5 && !b6 && b7 ? who + ":" + a : "badanim"); }
};
struct Def { const char* const* speaker; const char* const* dialogue; const char* const* actor; const char* const* anim; const float* delay; };
class CGameScriptInterface { public:
    int activeFrames; CScriptThing hero;
    CScriptThing& GetHero() { hero.name = "hero"; return hero; }
    CScriptThing GetThingWithScriptName(const std::string& n) { CScriptThing t; t.name = n; return t; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { Log(!a && !b ? "conv:" + t.name : "badconv"); return 9; }
    void AddPersonToConversation(long c, const CScriptThing& t) { Log(c == 9 ? "person:" + t.name : "badperson"); }
    void AddLineToConversation(long c, const std::string& s, bool b, const CScriptThing& sp, const CScriptThing& to)
    { Log(c == 9 && !b && to.name == "hero" ? "line:" + sp.name + ":" + s : "badline"); }
    void Pause(float f) { char buf[32]; std::sprintf(buf, "pause:%.1f", f); Log(buf); }
    bool IsConversationActive(long c) { return c == 9 && activeFrames-- > 0; }
    void NewScriptFrame() { Log("frame"); }
    void RemoveConversation(long c, bool b) { Log(c == 9 && !b ? "remove" : "badremove"); }
};
struct CV_TravellingHeroesScript {
    CGameScriptInterface* Game; const Def* def; Handle seh_Rival, seh_Girl, seh_Boy;
    bool ThunderKidsTalking, ThunderSceneStarted; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void DoConversation(long line)
    {
        std::string speaker = def->speaker[line], dialogue = def->dialogue[line], actor = def->actor[line], animation = def->anim[line];
        float delay = def->delay[line];
        CScriptThing speakerThing = Game->GetThingWithScriptName(speaker);
        long conversation = Game->AddNewConversation(speakerThing, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, dialogue, false, speakerThing, Game->GetHero());
        if (actor != "NULL") {
            if (IsActiveThreadTerminating()) return;
            Handle* actorHandle = 0;
            if (actor == "Thunder" || actor == "Whisper") { if (IsActiveThreadTerminating()) return; actorHandle = &seh_Rival; }
            else if (actor == "Boy") { if (IsActiveThreadTerminating()) return; actorHandle = &seh_Boy; }
            else if (actor == "Girl") { if (IsActiveThreadTerminating()) return; actorHandle = &seh_Girl; }
            if (actorHandle) {
                if (IsActiveThreadTerminating()) return;
                Game->Pause(delay);
                if (!ThunderSceneStarted) {
                    if (IsActiveThreadTerminating()) return;
                    actorHandle->PlayAnimation(animation, false, true, false, false, false, false, true);
                }
            }
        }
        while (Game->IsConversationActive(conversation) && !ThunderSceneStarted && !ThunderKidsTalking) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->RemoveConversation(conversation, false);
    }
};
static bool Run(long line, bool started, const char* expect)
{
    static const char* const sp[] = { "Thunder", "Whisper", "Girl", "Boy" };
    static const char* const dl[] = { "T0", "T1", "T2", "T3" };
    static const char* const ac[] = { "Whisper", "NULL", "Girl", "Nobody" };
    static const char* const an[] = { "wave", "nod", "laugh", "cry" };
    static const float dy[] = { 0.5f, 1.0f, 1.5f, 2.0f };
    static const Def def = { sp, dl, ac, an, dy };
    g_log[0] = 0;
    CGameScriptInterface g; g.activeFrames = 2;
    CV_TravellingHeroesScript s; s.Game = &g; s.def = &def; s.seh_Rival.who = "rival"; s.seh_Girl.who = "girl"; s.seh_Boy.who = "boy";
    s.ThunderKidsTalking = false; s.ThunderSceneStarted = started; s.termAfter = 20;
    s.DoConversation(line);
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return true;
}
int main()
{
    bool ok = Run(0, false, "conv:Thunder;person:hero;line:Thunder:T0;pause:0.5;rival:wave;frame;frame;remove;")
        && Run(1, false, "conv:Whisper;person:hero;line:Whisper:T1;frame;frame;remove;")
        && Run(2, true, "conv:Girl;person:hero;line:Girl:T2;pause:1.5;remove;")
        && Run(3, false, "conv:Boy;person:hero;line:Boy:T3;frame;frame;remove;");
    if (!ok) { std::printf("00ee2a80_TEST FAIL\n"); return 1; }
    std::printf("00ee2a80_TEST PASS\n");
    return 0;
}