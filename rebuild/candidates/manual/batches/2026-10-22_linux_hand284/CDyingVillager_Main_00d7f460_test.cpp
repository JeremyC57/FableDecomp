// Behaviour test for CDyingVillager::Main (0x00D7F460): mortally-wounded setup, dying loop until the hero is near,
// one comment line, the death animation, then PlayedOnce; a second run removes the villager first.
#include <cstdio>
#include <cstring>

static int g_frames, g_terminateAt, g_removed, g_lines, g_setupCalls, g_nearAt, g_convEndsAt;
static char g_lastAnim[40]; static char g_mode[40];
struct CScriptThing { float X; bool Friends; void SetFriendsWithEverythingFlag(bool on) { Friends = on; } };
struct Handle {
    bool scripting;
    Handle() : scripting(false) { }
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; }
    void PlayAnimation(const char* a, bool, bool, bool, bool, bool, bool, bool) { std::strcpy(g_lastAnim, a); }
    void PlayLoopingAnimation(const char* a, long, bool, bool, bool, bool, bool, bool, bool) { std::strcpy(g_lastAnim, a); }
};
struct GameCalls {
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frames; }
    bool StartScriptingEntity(const CScriptThing&, Handle& h, long) { h.scripting = true; return true; }
    CScriptThing& GetHero() { return Hero; }
    void RemoveThing(const CScriptThing&, bool, bool) const { ++g_removed; }
    void AddCreatureScriptedMode(const CScriptThing&, const char* m) const { std::strcpy(g_mode, m); }
    void Setup(const CScriptThing&) const { ++g_setupCalls; }
    long AddNewConversation(const CScriptThing&, bool, bool) const { return 7; }
    void AddPersonToConversation(long, const CScriptThing&) const { }
    void AddLineToConversation(long c, const char*, bool, const CScriptThing&, const CScriptThing&) const { if (c == 7) ++g_lines; }
    bool IsConversationActive(long) const { return g_frames < g_convEndsAt; }
};
static bool g_flag = false;
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 18.0f && g_frames >= g_nearAt; }
struct CDyingVillager {
    GameCalls* Game; CScriptThing Me; bool PlayedOnce;
    bool IsActiveThreadTerminating() { return g_frames >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Handle handle;
        if (handle.IsScripting()) handle.Release();
        while (!Game->StartScriptingEntity(Me, handle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (PlayedOnce) { if (IsActiveThreadTerminating()) return; Game->RemoveThing(Me, false, true); }
        Game->AddCreatureScriptedMode(Me, "MORTALLY_WOUNDED");
        Me.SetFriendsWithEverythingFlag(true);
        Game->Setup(Me); Game->Setup(Me); Game->Setup(Me); Game->Setup(Me); Game->Setup(Me);
        handle.PlayLoopingAnimation("CS_VILLAGER_DYING_CYCLE", -1, false, false, false, true, g_flag, false, false);
        Game->Setup(Me); Game->Setup(Me);
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 18.0f)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_DYINGVILLAGER_COMMENT_10", false, Me, Game->GetHero());
        while (Game->IsConversationActive(conversation)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        handle.PlayAnimation("VILLAGER_MALE_DYING", true, true, false, false, g_flag, false, false);
        PlayedOnce = true;
        do { Game->NewScriptFrame(); } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    GameCalls game; CDyingVillager v; v.Game = &game; v.PlayedOnce = false; v.Me.Friends = false;
    g_terminateAt = 30; g_nearAt = 5; g_convEndsAt = 9;
    v.Main();
    bool ok = v.PlayedOnce && g_lines == 1 && g_removed == 0 && v.Me.Friends && g_setupCalls == 7
        && std::strcmp(g_lastAnim, "VILLAGER_MALE_DYING") == 0 && std::strcmp(g_mode, "MORTALLY_WOUNDED") == 0 && g_frames == 30;
    g_frames = 0; g_terminateAt = 4; g_lines = 0;
    v.Main();
    ok = ok && g_removed == 1 && g_lines == 0 && std::strcmp(g_lastAnim, "CS_VILLAGER_DYING_CYCLE") == 0;
    if (!ok) { std::printf("00d7f460_TEST FAIL\n"); return 1; }
    std::printf("00d7f460_TEST PASS\n");
    return 0;
}
