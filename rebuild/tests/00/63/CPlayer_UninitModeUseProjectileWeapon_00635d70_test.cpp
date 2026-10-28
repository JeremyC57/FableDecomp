// Behaviour test for 00635d70: CPlayer::UninitModeUseProjectileWeapon -- remove the eleven projectile-mode input
// processes in retail order; for a living controlled creature take its mode manager out of mode 25 and clear
// ProjectileTargetLocked; push the camera's manual mode when the game camera has a manager; drop player mode 25 when
// the new mode is 25 or 2.
#include <cstdio>

static int g_order[16];
static int g_removed = 0;
struct InputProcess { int id; void RemoveFromInterface() { g_order[g_removed++] = id; } };
struct ModeManager { int removed; void RemoveMode(int m) { removed = m; } };
struct Creature { bool dead; ModeManager modes; };
struct CameraManager { int pushes; void PushCurrentManualMode() { ++pushes; } };
struct Camera { CameraManager* manager; bool GetCameraManager(CameraManager** out) { *out = manager; return manager != 0; } };

struct Player {
    InputProcess processes[11];
    Creature* creature;
    Camera* camera;
    bool ProjectileTargetLocked;
    int removedPlayerMode;
    void RemoveMode(int m) { removedPlayerMode = m; }
    void UninitModeUseProjectileWeapon(int newMode)
    {
        for (int i = 0; i < 11; ++i)
            processes[i].RemoveFromInterface();
        Creature* hero = creature;
        if (hero && !hero->dead) {
            hero->modes.RemoveMode(25);
            ProjectileTargetLocked = false;
        }
        CameraManager* manager = 0;
        if (camera->GetCameraManager(&manager))
            manager->PushCurrentManualMode();
        if (newMode == 25 || newMode == 2)
            RemoveMode(25);
    }
};

static Player make(Creature* c, Camera* cam)
{
    Player p;
    for (int i = 0; i < 11; ++i) p.processes[i].id = i;
    p.creature = c; p.camera = cam; p.ProjectileTargetLocked = true; p.removedPlayerMode = -1;
    return p;
}

int main()
{
    bool ok = true;
    Creature alive = { false, { 0 } }, dead = { true, { 0 } };
    CameraManager mgr = { 0 };
    Camera withMgr = { &mgr }, noMgr = { 0 };

    Player p = make(&alive, &withMgr);
    g_removed = 0;
    p.UninitModeUseProjectileWeapon(2);
    for (int i = 0; i < 11; ++i) ok = ok && g_order[i] == i;
    ok = ok && g_removed == 11 && alive.modes.removed == 25 && !p.ProjectileTargetLocked && mgr.pushes == 1
            && p.removedPlayerMode == 25;

    Player q = make(&dead, &noMgr);
    g_removed = 0;
    q.UninitModeUseProjectileWeapon(7);
    ok = ok && dead.modes.removed == 0 && q.ProjectileTargetLocked && mgr.pushes == 1 && q.removedPlayerMode == -1;

    Player r = make(0, &withMgr);
    g_removed = 0;
    r.UninitModeUseProjectileWeapon(25);
    ok = ok && r.ProjectileTargetLocked && mgr.pushes == 2 && r.removedPlayerMode == 25 && g_removed == 11;
    if (!ok) { std::printf("00635d70_TEST FAIL\n"); return 1; }
    std::printf("00635d70_TEST PASS\n");
    return 0;
}