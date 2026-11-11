// Behaviour test for 0x00edc980 NScript::CV_TempleOfLightScript::WatchForAcolyteDeath(CScriptThing): the row's body (without its codegen-only
// inline_depth pragmas) over test doubles; checks every exit's call sequence: predicate, first frame, killed / hit-and-dying /
// alive-for-a-frame paths, and the terminate checks between them.
#include <cstdio>
#include <cstring>
static int g_log[256]; static int g_n = 0;
static void Log(int v) { if (g_n < 256) g_log[g_n++] = v; }
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
struct SFrame { bool Killed, Hit, HitSpecial; float Health; };
static const SFrame* g_frames = 0; static int g_frame = 0;
class CScriptThing { public:
    bool Null;
    bool MsgIsKilledBy(const CCharString& who) const { Log(std::strlen(who.Text) == 0 ? 20 : 99); return !Null && g_frames[g_frame].Killed; }
    bool MsgIsHitBy(const CCharString& who) const { Log(std::strcmp(who.Text, "SCRIPT_NAME_HERO") == 0 ? 21 : 99); return !Null && g_frames[g_frame].Hit; }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& who) const { Log(std::strcmp(who.Text, "SCRIPT_NAME_HERO") == 0 ? 22 : 99); return !Null && g_frames[g_frame].HitSpecial; }
};
struct CIsThingAlive { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) {} };
class CGame { public:
    void NewScriptFrame() const { Log(5); ++g_frame; }
    float GetHealth(const CScriptThing&) const { Log(30); return g_frames[g_frame].Health; }
};
static int g_terminateAt = 0, g_checks = 0;
struct CV_TempleOfLightScript {
    CGame* Game;
    void SetPredicate(const CIsThingAlive&) { Log(8); }
    bool IsActiveThreadTerminating() { Log(9); return ++g_checks >= g_terminateAt; }
    void WatchForAcolyteDeath(CScriptThing acolyte);
};
void CV_TempleOfLightScript::WatchForAcolyteDeath(CScriptThing acolyte)
{
    SetPredicate(CIsThingAlive(acolyte));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool killed = acolyte.MsgIsKilledBy("");
        if (killed) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        bool hit = acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->GetHealth(acolyte) < 0.0001f) {
                if (IsActiveThreadTerminating())
                    return;
                break;
            }
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
}
static bool Run(const SFrame* frames, bool null, int terminateAt, const int* want, int nwant)
{
    CGame game; CV_TempleOfLightScript s; s.Game = &game; CScriptThing t; t.Null = null;
    g_frames = frames; g_frame = 0; g_n = 0; g_terminateAt = terminateAt; g_checks = 0;
    s.WatchForAcolyteDeath(t);
    if (g_n != nwant) return false;
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    // frame 0 is consumed by the first NewScriptFrame
    static const SFrame killed[] = { {false,false,false,1}, {false,false,false,1}, {true,false,false,1} };
    static const SFrame dying[]  = { {false,false,false,1}, {false,true,false,0.5f}, {false,false,true,0.00001f} };
    static const int a[] = { 8, 5, 9 };                                             // terminating after the first frame
    static const int b[] = { 8, 5, 9, 9, 20, 21, 22, 5, 9, 20, 9, 9 };             // alive one frame, then killed; trailing check
    static const int c[] = { 8, 5, 9, 9, 20, 21, 9, 30, 5, 9, 20, 21, 22, 9, 30, 9, 9 };   // hit with health, then special hit and dying
    static const int d[] = { 8, 5, 9, 9, 20, 21, 9 };                              // terminating right after the hit
    static const int e[] = { 8, 5, 9, 9, 20, 21, 22, 5, 9, 20, 21, 22, 5, 9 };      // null thing: nothing ever happens
    bool ok = Run(killed, false, 1, a, 3) && Run(killed, false, 100, b, 12) && Run(dying, false, 100, c, 17)
           && Run(dying, false, 3, d, 7) && Run(killed, true, 4, e, 14);
    if (!ok) { std::printf("00edc980_TEST FAIL\n"); return 1; }
    std::printf("00edc980_TEST PASS\n");
    return 0;
}