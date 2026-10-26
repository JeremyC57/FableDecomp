// Behaviour test for CPlayer::InitCharacterAs (0x0048A070): the thing manager / world map pointers are refreshed, the
// character is (re)created only when missing or dead, progress is reported, the character becomes the controlled
// creature; the main player points the camera at it and resets the camera, a co-op player registers it with the main
// player's controlled creature when that creature has the 0x7a component.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
static void DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2)
{ if (std::strcmp(text.t, "CPlayer::InitCharacterAs") == 0 && !flag && fraction == -1.0f && !flag2) Log("progress"); }
class CThing { public: bool dead; bool has7a; };
struct CTCPlayerCharacterLinks { void AddThing(CThing* t) { (void)t; Log("link"); } };
static CTCPlayerCharacterLinks g_links;
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
class CGameCameraManager { public: CThing* followed; void SetFollowedThing(CThing* t) { followed = t; Log("follow"); } void ResetCamera() { Log("reset"); } };
static CGameCameraManager g_cam;
class CThingManager { public: int x; }; class CWorldMap { public: int y; };
static CThingManager g_tm; static CWorldMap g_wm;
class CWorld { public: CThingManager* GetThingManager() { return &g_tm; } CWorldMap* GetWorldMap() { return &g_wm; }
    bool GetCameraManager(CGameCameraManager** c) { *c = &g_cam; return true; } };
class CPlayer;
class CPlayerManager { public: long mainID; CPlayer* main; long GetMainPlayerID() const { return mainID; } CPlayer* GetMainPlayer() { return main; } };
class CPlayer { public:
    CPlayerManager* PlayerManager; CWorld* World; CThingManager* ThingManager; CWorldMap* PWorldMap; long Number;
    CPtr ControlledCreaturePtr; CPtr PlayerCharacterPtr; CThing* newChar;
    bool CreateCharacter(const CCharString& d) { (void)d; PlayerCharacterPtr.item = newChar; Log("create"); return true; }
    void SetControlledCreature(CThing* c) { ControlledCreaturePtr.item = c; Log("control"); }
    void InitCharacterAs(const CCharString& defName);
};
void CPlayer::InitCharacterAs(const CCharString& defName)
{
    ThingManager = World->GetThingManager();
    PWorldMap = World->GetWorldMap();
    if (PlayerCharacterPtr.GetPItem() == 0 || PlayerCharacterPtr.GetPItem()->dead)
        CreateCharacter(defName);
    DisplayProgress("CPlayer::InitCharacterAs", false, -1.0f, false);
    SetControlledCreature(PlayerCharacterPtr.GetPItem());
    long number = Number;
    if (number == PlayerManager->GetMainPlayerID()) {
        CGameCameraManager* camera;
        World->GetCameraManager(&camera);
        camera->SetFollowedThing(PlayerCharacterPtr.GetPItem());
        camera->ResetCamera();
        return;
    }
    if (PlayerManager->GetMainPlayer()->ControlledCreaturePtr.GetPItem() != 0) {
        CThing* creature = PlayerManager->GetMainPlayer()->ControlledCreaturePtr.GetPItem();
        if (creature->has7a)
            g_links.AddThing(PlayerCharacterPtr.GetPItem());
    }
}
int main()
{
    int bad = 0;
    CWorld w; CThing c1 = { false, true }, c2 = { false, false }, dead = { true, false };
    CPlayerManager pm; CPlayer main; std::memset(&main, 0, sizeof(main));
    main.PlayerManager = &pm; main.World = &w; main.Number = 0; main.newChar = &c1;
    pm.mainID = 0; pm.main = &main;
    g_log[0] = 0; main.InitCharacterAs(CCharString("HERO"));
    if (std::strcmp(g_log, "create;progress;control;follow;reset;") != 0 || g_cam.followed != &c1 || main.ThingManager != &g_tm || main.PWorldMap != &g_wm) bad = 1;
    CPlayer coop; std::memset(&coop, 0, sizeof(coop));
    coop.PlayerManager = &pm; coop.World = &w; coop.Number = 1; coop.PlayerCharacterPtr.item = &c2;
    g_log[0] = 0; coop.InitCharacterAs(CCharString("HENCH"));
    if (std::strcmp(g_log, "progress;control;link;") != 0 || coop.ControlledCreaturePtr.item != &c2) bad = 1;
    coop.PlayerCharacterPtr.item = &dead; coop.newChar = &c2; main.ControlledCreaturePtr.item = &c2;
    g_log[0] = 0; coop.InitCharacterAs(CCharString("HENCH"));
    if (std::strcmp(g_log, "create;progress;control;") != 0) bad = 1;
    if (bad) { std::printf("0048a070_TEST FAIL\n"); return 1; }
    std::printf("0048a070_TEST PASS\n");
    return 0;
}