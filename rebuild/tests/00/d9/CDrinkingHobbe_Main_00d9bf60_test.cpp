// Behaviour test for CDrinkingHobbe::Main (0x00D9BF60): scripting is retried until granted, the hobbe waits to be hit,
// then targets the nearer of the hero and the whisper, and idles until the thread terminates.
#include <cstdio>
#include <cstring>

static int g_frames, g_terminateAt, g_grantAt, g_hitAt, g_releases;
static char g_target[32];
class CScriptThing { public:
    float X; char Name[32];
    bool MsgIsHitBy(const char*) const { return g_frames >= g_hitAt; }
};
static float Distance(const CScriptThing& a, const CScriptThing& b) { float d = b.X - a.X; return d < 0 ? -d : d; }
struct Handle {
    bool scripting;
    Handle() : scripting(false) { }
    bool IsScripting() { return scripting; }
    void Release() { scripting = false; ++g_releases; }
};
struct GameCalls {
    CScriptThing Hero, Whisper;
    void NewScriptFrame() const { ++g_frames; }
    bool StartScriptingEntity(const CScriptThing&, Handle& h, long) { if (g_frames < g_grantAt) return false; h.scripting = true; return true; }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const char* name) const { CScriptThing t = Whisper; std::strcpy(t.Name, name); return t; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) const { std::strcpy(g_target, t.Name); }
};
struct Parent { bool DrinkingHobbesNotHit; };
struct CDrinkingHobbe {
    GameCalls* Game; CScriptThing Me; Parent* ParentClass;
    bool IsActiveThreadTerminating() { return g_frames >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Handle handle;
        if (handle.IsScripting()) handle.Release();
        while (!Game->StartScriptingEntity(Me, handle, 4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->DrinkingHobbesNotHit) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->DrinkingHobbesNotHit = false;
            }
        }
        if (IsActiveThreadTerminating()) return;
        float heroDistance = Distance(Me, Game->GetHero());
        float whisperDistance = Distance(Me, Game->GetThingWithScriptName("Q_HobbeToothWhisper"));
        if (whisperDistance < heroDistance) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetThingWithScriptName("Q_HobbeToothWhisper"));
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        }
        if (handle.IsScripting()) handle.Release();
        do { Game->NewScriptFrame(); } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    GameCalls game; game.Hero.X = 10.0f; std::strcpy(game.Hero.Name, "HERO"); game.Whisper.X = 4.0f;
    Parent parent; parent.DrinkingHobbesNotHit = true;
    CDrinkingHobbe h; h.Game = &game; h.Me.X = 0.0f; h.ParentClass = &parent;
    g_terminateAt = 20; g_grantAt = 3; g_hitAt = 6;
    h.Main();
    bool ok = !parent.DrinkingHobbesNotHit && std::strcmp(g_target, "Q_HobbeToothWhisper") == 0 && g_releases == 1 && g_frames == 20;
    g_frames = 0; parent.DrinkingHobbesNotHit = true; game.Whisper.X = 30.0f; g_releases = 0;
    h.Main();
    ok = ok && std::strcmp(g_target, "HERO") == 0 && g_releases == 1;
    g_frames = 0; g_terminateAt = 2; parent.DrinkingHobbesNotHit = true; g_target[0] = 0;
    h.Main();
    ok = ok && parent.DrinkingHobbesNotHit && g_target[0] == 0;
    if (!ok) { std::printf("00d9bf60_TEST FAIL\n"); return 1; }
    std::printf("00d9bf60_TEST PASS\n");
    return 0;
}