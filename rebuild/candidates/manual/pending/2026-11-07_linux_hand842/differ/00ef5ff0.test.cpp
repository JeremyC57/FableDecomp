// Behaviour test for 0x00ef5ff0 NScript::CPersonalScript_EmptyScript::CCinemaMan::Main: look up the three cinema markers and both
// look-at targets, guard the thread with "all six alive", take scripted control, then each frame answer an aggressive special
// ability hit ("How very mean!"), else a non-aggressive one ("How very nice!"), else a hero chat (paused movie, the eight
// TEXT_QST_048_FATHER_INTRO lines while alive, each waited out).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static int g_frame, g_endFrame, g_starts, g_movies, g_pauses, g_lookups; static bool g_valid;
static int g_meanFrame = -1, g_niceFrame = -1, g_talkFrame = -1, g_busyUntil; static float g_health = 1.0f;
static std::vector<std::string> g_messages, g_lines; static int g_predicateParts;
struct CScriptThing { int Id; bool IsAlive() const { return Id != 0; } };
static bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
static void NewScriptFrame() { ++g_frame; }
static CScriptThing GetThingWithScriptName(const char*) { ++g_lookups; CScriptThing t; t.Id = g_lookups; return t; }
static void SetPredicate(const CScriptThing* things, int n) { for (int i = 0; i < n; ++i) if (things[i].IsAlive()) ++g_predicateParts; }
struct CSeh {
    bool IsValid() const { return g_valid; }
    void Release() { g_valid = false; }
    void Speak(const char* key) { g_lines.push_back(key); g_busyUntil = g_frame + 2; }
    bool IsPerformingScriptTask() const { return g_frame < g_busyUntil; }
};
static bool StartScriptingEntity(CSeh&) { ++g_starts; g_valid = true; return g_starts == 2; }
struct CPause { CPause() { ++g_pauses; } ~CPause() { --g_pauses; } };
#define CM_T if (IsActiveThreadTerminating()) return
#define CM_SPEAK(key) \
    if (g_health > 0.0f) { seh_me.Speak(key); while (seh_me.IsPerformingScriptTask()) { NewScriptFrame(); CM_T; } CM_T; }

static void Main()
{
    CScriptThing all[6]; all[0].Id = 99;
    all[1] = GetThingWithScriptName("CinemaMarker1"); all[2] = GetThingWithScriptName("CinemaMarker2");
    all[3] = GetThingWithScriptName("CinemaMarker3"); all[4] = GetThingWithScriptName("LookAtMe1"); all[5] = GetThingWithScriptName("LookAtMe2");
    SetPredicate(all, 6);
    NewScriptFrame();
    CM_T;
    CSeh seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!StartScriptingEntity(seh_me)) { NewScriptFrame(); CM_T; }
    CM_T;
    while (!IsActiveThreadTerminating()) {
        if (g_frame == g_meanFrame) { CM_T; g_messages.push_back("How very mean!"); }
        else if (g_frame == g_niceFrame) { CM_T; g_messages.push_back("How very nice!"); }
        else if (g_frame == g_talkFrame) {
            CM_T;
            ++g_movies; CPause PauseMyEntitiesPlease;
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_10"); CM_SPEAK("TEXT_QST_048_FATHER_INTRO_20");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_30"); CM_SPEAK("TEXT_QST_048_FATHER_INTRO_60");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_70"); CM_SPEAK("TEXT_QST_048_FATHER_INTRO_80");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_90"); CM_SPEAK("TEXT_QST_048_FATHER_INTRO_100");
            while (seh_me.IsPerformingScriptTask()) { NewScriptFrame(); CM_T; }
            CM_T;
        }
        NewScriptFrame();
    }
}

int main()
{
    g_endFrame = 40; g_meanFrame = 3; g_niceFrame = 4; g_talkFrame = 5;
    Main();
    bool ok = g_lookups == 5 && g_predicateParts == 6 && g_starts == 2 && g_frame == 40;
    ok = ok && g_messages.size() == 2 && g_messages[0] == "How very mean!" && g_messages[1] == "How very nice!";
    ok = ok && g_movies == 1 && g_pauses == 0 && g_lines.size() == 8 && g_lines[3] == "TEXT_QST_048_FATHER_INTRO_60"
            && g_lines[7] == "TEXT_QST_048_FATHER_INTRO_100";
    // terminated in the middle of the chat: the pause is undone, the rest is not spoken
    g_frame = 0; g_starts = 0; g_lookups = 0; g_predicateParts = 0; g_lines.clear(); g_messages.clear(); g_movies = 0;
    g_endFrame = 9; g_meanFrame = g_niceFrame = -1; g_talkFrame = 2; g_busyUntil = 0;
    Main();
    ok = ok && g_movies == 1 && g_pauses == 0 && g_lines.size() == 4 && g_messages.empty();
    std::printf(ok ? "00ef5ff0_TEST PASS\n" : "00ef5ff0_TEST FAIL\n");
    return ok ? 0 : 1;
}
