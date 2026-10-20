// Behaviour test for CPrisonKeyDoor::Main (0x00DD9180): the door is locked, a hero use shows the locked message,
// waits until it is clicked past and pauses 5 s; once the key is stolen the door is unlocked; a done thread stops it.
#include <cstdio>
#include <cstring>

static int g_live = 0;
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* t, long = -1) : PStringData(t) { ++g_live; }
    ~CCharString() { --g_live; }
};
static const char* g_used; static const char* g_clicked; static const char* g_done;
static bool Next(const char*& p, char yes) { char c = *p; if (c) ++p; return c == yes; }
struct CQ_SunnyvaleMasterData { bool PrisonKeyStolenByHero; };
static CQ_SunnyvaleMasterData g_md;
struct CScriptThing {
    virtual ~CScriptThing() {}
    virtual bool MsgIsUsedBy(const CCharString& n) const { return std::strcmp(n.PStringData, "SCRIPT_NAME_HERO") == 0 && Next(g_used, '1'); }
};
static int g_frames = 0;
struct CGameScriptInterface {
    virtual void NewScriptFrame() { ++g_frames; if (g_frames == 4) g_md.PrisonKeyStolenByHero = true; }
    virtual bool MsgIsGameInfoClickedPast() { return Next(g_clicked, '1'); }
    virtual void DisplayGameInfo(const CCharString& t) { Log(t.PStringData); }
    virtual void Pause(float s) { Log(s == 5.0f ? "pause5" : "pause?"); }
    virtual void EntitySetAsLocked(const CScriptThing&, bool l) { Log(l ? "lock" : "unlock"); }
};
class CPrisonKeyDoor {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    CQ_SunnyvaleMasterData* PMasterData;
    bool IsActiveThreadTerminating() { return Next(g_done, '1'); }
    void Main();
};
void CPrisonKeyDoor::Main()
{
    Game->EntitySetAsLocked(Me, true);
    while (!PMasterData->PrisonKeyStolenByHero) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_WARDENS_DOOR_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(5.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsLocked(Me, false);
}
int main()
{
    int bad = 0;
    CGameScriptInterface gi;
    CPrisonKeyDoor d; d.Game = &gi; d.PMasterData = &g_md;
    // frame 1: used -> message, click wait frame 2, clicked, pause; frame 3: not used; frame 4 steals the key
    g_md.PrisonKeyStolenByHero = false; g_frames = 0; g_log[0] = 0;
    g_used = "100"; g_clicked = "01"; g_done = "0000000";
    d.Main();
    if (std::strcmp(g_log, "lock;TEXT_QST_055_WARDENS_DOOR_LOCKED;pause5;unlock;") != 0 || g_frames != 4 || *g_done || g_live != 0) bad = 1;
    // done on the first frame
    g_md.PrisonKeyStolenByHero = false; g_frames = 0; g_log[0] = 0; g_used = ""; g_clicked = ""; g_done = "1";
    d.Main();
    if (std::strcmp(g_log, "lock;") != 0 || g_frames != 1) bad = 1;
    if (bad) { std::printf("00dd9180_TEST FAIL\n"); return 1; }
    std::printf("00dd9180_TEST PASS\n");
    return 0;
}