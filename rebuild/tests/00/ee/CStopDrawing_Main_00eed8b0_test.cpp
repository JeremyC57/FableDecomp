#include <cstdio>
#include <cstring>
// behaviour test: CStopDrawing::Main hides Me when touched by the hero, then keeps framing until done
class CCharString { public: const char* text; CCharString(const char* t, long = -1) : text(t) {} };
static int g_frames = 0, g_doneAfter = 0;
static bool g_talk = false, g_hit = false, g_any = false, g_special14 = false;
static bool isHero(const CCharString& s) { return std::strcmp(s.text, "SCRIPT_NAME_HERO") == 0; }
class CScriptThing { public:
    bool MsgIsHitBy(const CCharString& h) const { return isHero(h) && g_hit; }
    bool MsgIsTalkedToBy(const CCharString& t) const { return isHero(t) && g_talk; }
    bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& f) const { return ability == 0xe && isHero(f) && g_special14; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& f) const { return isHero(f) && g_any; }
};
static int g_hidden = 0, g_filters = 0;
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    void EntitySetAsDrawable(const CScriptThing&, bool drawable) const { if (!drawable) ++g_hidden; }
};
struct CIsThingAlive { CIsThingAlive(const CScriptThing&) {} };
static const char g_scriptNameHero[] = "SCRIPT_NAME_HERO";
class CStopDrawing {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    void RegisterFilter(CIsThingAlive&) { ++g_filters; }
    bool IsDone() { return g_frames >= g_doneAfter; }
    void Main();
};
void CStopDrawing::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {
        bool touched = Me.MsgIsTalkedToBy(g_scriptNameHero) || Me.MsgIsHitBy(g_scriptNameHero)
                    || (Me.MsgIsHitByAnySpecialAbilityFrom(g_scriptNameHero)
                        && !Me.MsgIsHitBySpecialAbilityFrom(0xe, g_scriptNameHero));
        if (touched) {
            if (IsDone())
                return;
            Game->EntitySetAsDrawable(Me, false);
        }
        Game->NewScriptFrame();
    }
}

static int run(bool talk, bool hit, bool any, bool sp14, int doneAfter)
{
    g_talk = talk; g_hit = hit; g_any = any; g_special14 = sp14; g_frames = 0; g_doneAfter = doneAfter; g_hidden = 0;
    CGameScriptInterface game; CStopDrawing s; s.Game = &game; s.Main();
    return g_hidden;
}
int main()
{
    bool ok = run(true, false, false, false, 4) == 3 && g_frames == 4
           && run(false, true, false, false, 3) == 2
           && run(false, false, true, false, 3) == 2
           && run(false, false, true, true, 3) == 0
           && run(false, false, false, false, 5) == 0 && g_frames == 5
           && run(true, true, true, true, 1) == 0 && g_frames == 1 && g_filters == 6;
    if (!ok) { std::printf("00eed8b0_TEST FAIL\n"); return 1; }
    std::printf("00eed8b0_TEST PASS\n");
    return 0;
}