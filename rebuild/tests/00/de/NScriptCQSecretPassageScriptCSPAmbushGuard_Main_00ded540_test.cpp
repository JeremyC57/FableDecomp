// Behaviour test for CSP_AmbushGuard::Main (0x00DED540): filter registered, scripting retried until granted, wait until
// the hero sees or hears the guard, then one conversation line and the hero as best enemy target; handle released.
#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[48];
    CCharString(const char* text) { std::strncpy(Text, text, 47); Text[47] = 0; }
};
static int g_frames = 0;
class CScriptThing { public: int id; };
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) { } };
class CScriptGameResourceObjectScriptedThingBase { public:
    bool scripting; int releases;
    CScriptGameResourceObjectScriptedThingBase() : scripting(false), releases(0) { }
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; ++releases; }
};
static int g_handleReleases = 0;
class CGameScriptInterface { public:
    int grantAt, seenAt, heardAt, lines, targets, conversations; CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, int priority) {
        if (priority != 4 || t.id != 1 || g_frames < grantAt) return false; h.scripting = true; return true;
    }
    CScriptThing& GetHero() { return hero; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { if (t.id == 1 && !a && !b) ++conversations; return 77; }
    void AddLineToConversation(long c, const CCharString& line, bool f, const CScriptThing& s, const CScriptThing& l) {
        if (c == 77 && !f && s.id == 1 && l.id == 9 && std::strcmp(line.Text, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO") == 0) ++lines;
    }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& target) { if (t.id == 1 && target.id == 9) ++targets; }
    bool CanThingBe_Seen_ByOtherThing(const CScriptThing& t, const CScriptThing& o) const { return t.id == 9 && o.id == 1 && g_frames >= seenAt; }
    bool CanThingBe_Heard_ByOtherThing(const CScriptThing& t, const CScriptThing& o) const { return t.id == 9 && o.id == 1 && g_frames >= heardAt; }
};
struct CSP_AmbushGuard {
    CGameScriptInterface* Game; CScriptThing Me; int doneAt; int filters;
    void RegisterFilter(CIsThingAlive& f) { if (f.m_thing.id == Me.id) ++filters; }
    bool IsDone() { return g_frames >= doneAt; }
    void Main();
};
void CSP_AmbushGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, 4)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    while (!(Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) || Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(), Me))) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO", false, Me, Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (handle.IsScripting())
        handle.Release();
    g_handleReleases = handle.releases;
}
int main()
{
    CGameScriptInterface g; std::memset(&g, 0, sizeof(g)); g.grantAt = 3; g.seenAt = 50; g.heardAt = 6; g.hero.id = 9;
    CSP_AmbushGuard s; s.Game = &g; s.Me.id = 1; s.doneAt = 100; s.filters = 0;
    s.Main();
    bool ok = s.filters == 1 && g_frames == 6 && g.conversations == 1 && g.lines == 1 && g.targets == 1 && g_handleReleases == 1;
    g_frames = 0; std::memset(&g, 0, sizeof(g)); g.grantAt = 100; g.hero.id = 9;
    CSP_AmbushGuard t; t.Game = &g; t.Me.id = 1; t.doneAt = 4; t.filters = 0;
    t.Main();
    ok = ok && g_frames == 4 && g.conversations == 0 && g.targets == 0;
    if (!ok) { std::printf("00ded540_TEST FAIL\n"); return 1; }
    std::printf("00ded540_TEST PASS\n");
    return 0;
}