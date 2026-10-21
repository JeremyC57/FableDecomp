// Behaviour test for 00633ff0 (CPlayer::InitModeQuickAccessMenu): every process gets AddToInterface(PlayerInterface) in order; with a player
// character a third-person view is set, ControlledMovementType becomes 1 and a living controlled creature's hero gets
// SetFacingAngleYZ(0).
#include <cstdio>

struct CGamePlayerInterface { int id; };
static int g_order[64]; static int g_n = 0; static int g_bad = 0;
struct CAInputProcess {
    int id;
    virtual bool AddToInterface(CGamePlayerInterface* pi) { if (pi->id != 7) g_bad = 1; g_order[g_n++] = id; return true; }
    virtual ~CAInputProcess() {}
};
struct CPhysics { float last; int calls; CPhysics() : last(-1.0f), calls(0) {} void SetFacingAngleYZ(float a) { last = a; ++calls; } };
struct CThing { CPhysics* PhysicsTC; unsigned char m_stateFlags; long ControlledMovementType; };
static bool IsThingAlive(const CThing* t) { return t && !(t->m_stateFlags & 1); }
static int g_setView = 0;
struct CPlayerModel {
    CGamePlayerInterface* PlayerInterface;
    CAInputProcess* procs[15];
    CThing* PlayerCharacter; CThing* Controlled; CThing* Hero;
    void Run()
    {
        for (int i = 0; i < 15; ++i) procs[i]->AddToInterface(PlayerInterface);
        if (PlayerCharacter) {
            ++g_setView;
            PlayerCharacter->ControlledMovementType = 1;
            if (IsThingAlive(Controlled)) Hero->PhysicsTC->SetFacingAngleYZ(0.0f);
        }
    }
};
int main()
{
    CGamePlayerInterface pi = { 7 };
    CAInputProcess p[15];
    CPhysics phys; CThing hero = { &phys, 0, 0 }; CThing pc = { 0, 0, 5 }; CThing ctl = { 0, 0, 0 };
    CPlayerModel m; m.PlayerInterface = &pi; m.PlayerCharacter = &pc; m.Controlled = &ctl; m.Hero = &hero;
    for (int i = 0; i < 15; ++i) { p[i].id = i; m.procs[i] = &p[i]; }
    m.Run();
    int ok = (g_n == 15) && !g_bad && g_setView == 1 && pc.ControlledMovementType == 1 && phys.calls == 1 && phys.last == 0.0f;
    for (int i = 0; i < g_n; ++i) if (g_order[i] != i) ok = 0;
    ctl.m_stateFlags = 1; g_n = 0; m.Run();
    if (phys.calls != 1 || g_setView != 2) ok = 0;
    m.PlayerCharacter = 0; g_n = 0; m.Run();
    if (g_setView != 2 || g_n != 15) ok = 0;
    if (!ok) { std::printf("00633ff0_TEST FAIL\n"); return 1; }
    std::printf("00633ff0_TEST PASS\n");
    return 0;
}