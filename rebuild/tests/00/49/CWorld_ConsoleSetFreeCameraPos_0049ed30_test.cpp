#include <cstdio>

// Behaviour model of CWorld::ConsoleSetFreeCameraPos (0x0049ED30): main player's free camera moved to the three float args.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CCamera {
    C3DVector WorldPosition;
    float     Zoom;
    CCamera() : WorldPosition(0.0f, 0.0f, 0.0f), Zoom(1.0f) { }
    void MoveTo(const C3DVector& pos) { WorldPosition = pos; }
};
struct CPlayer { long PlayerRespawnDelay; CCamera CurrentFreeCamera; };
static int g_calls = 0;
struct CPlayerManager {
    CPlayer m_players[2];
    long    m_main;
    CPlayer& GetMainPlayer() { ++g_calls; return m_players[m_main]; }
};
struct CWorld { CPlayerManager* PlayerManager; };
struct CGenericVar { long Type; long ValSLONG; float ValFloat; };
struct CGenericVarVector {
    CGenericVar* _M_start;
    CGenericVar* _M_finish;
    const CGenericVar& operator[](unsigned int n) const { return *(_M_start + n); }
};

static void ConsoleSetFreeCameraPos(CWorld& world, const CGenericVarVector& args)
{
    CPlayer& player = world.PlayerManager->GetMainPlayer();
    player.CurrentFreeCamera.MoveTo(C3DVector(args[0].ValFloat, args[1].ValFloat, args[2].ValFloat));
}

int main()
{
    CPlayerManager pm; pm.m_main = 1; pm.m_players[0].PlayerRespawnDelay = 0; pm.m_players[1].PlayerRespawnDelay = 0;
    CWorld world; world.PlayerManager = &pm;
    CGenericVar v[3] = { { 3, 0, 10.5f }, { 3, 0, -2.25f }, { 3, 0, 300.0f } };
    CGenericVarVector args; args._M_start = v; args._M_finish = v + 3;
    ConsoleSetFreeCameraPos(world, args);
    const C3DVector& p = pm.m_players[1].CurrentFreeCamera.WorldPosition;
    if (g_calls != 1 || p.X != 10.5f || p.Y != -2.25f || p.Z != 300.0f) { std::printf("0049ed30_TEST FAIL\n"); return 1; }
    if (pm.m_players[0].CurrentFreeCamera.WorldPosition.X != 0.0f || pm.m_players[1].CurrentFreeCamera.Zoom != 1.0f) { std::printf("0049ed30_TEST FAIL\n"); return 1; }
    std::printf("0049ed30_TEST PASS\n");
    return 0;
}