// Behaviour test for 0x0087C150 NCameraModes::COrbitGame::Update: with the hero's game-player component in a game, the
// coin-golf component of the game's thing supplies the camera position (-> GoTo) and target (-> LookAt) and the observed
// Z offset is cleared; without a current game nothing changes.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CTCTavernGameCoinGolf { public:
    C3DVector pos, target;
    void GetCamera(C3DVector& p, C3DVector& t) { p = pos; t = target; }
};
class CThing { public: void* components[256]; };
template <class T> static void GetInterface(CThing* thing, T** out, long type) { *out = (T*)thing->components[type]; }
class CTCTavernGame { public: CThing* ParentThing; };
class CTCGamePlayer { public: CTCTavernGame* game; CTCTavernGame* GetGame() const { return game; } };
struct COrbitGame {
    C3DVector LookAt, GoTo; float ObservedPositionZOffset; CThing* hero;
    void Update()
    {
        CTCGamePlayer* player;
        GetInterface(hero, &player, 0xd0);
        if (player && player->GetGame()) {
            CTCTavernGameCoinGolf* golf;
            GetInterface(player->GetGame()->ParentThing, &golf, 0xcf);
            C3DVector position, target;
            golf->GetCamera(position, target);
            ObservedPositionZOffset = 0.0f;
            GoTo = position;
            LookAt = target;
        }
    }
};
static CThing g_hero, g_table;
int main()
{
    CTCTavernGameCoinGolf golf = { { 1, 2, 3 }, { 4, 5, 6 } };
    CTCTavernGame game = { &g_table };
    CTCGamePlayer player = { 0 };
    g_hero.components[0xd0] = &player; g_table.components[0xcf] = &golf;
    COrbitGame cam; cam.hero = &g_hero; cam.ObservedPositionZOffset = 7.0f;
    cam.LookAt.X = cam.GoTo.X = -1.0f;
    cam.Update();
    if (cam.ObservedPositionZOffset != 7.0f || cam.GoTo.X != -1.0f) { std::printf("0087c150_TEST FAIL\n"); return 1; }
    player.game = &game; cam.Update();
    if (cam.ObservedPositionZOffset != 0.0f || cam.GoTo.X != 1 || cam.GoTo.Z != 3 || cam.LookAt.X != 4 || cam.LookAt.Z != 6) { std::printf("0087c150_TEST FAIL\n"); return 1; }
    std::printf("0087c150_TEST PASS\n");
    return 0;
}