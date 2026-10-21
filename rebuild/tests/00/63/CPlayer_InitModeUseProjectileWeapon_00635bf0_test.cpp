// Behaviour test for 00635bf0 (CPlayer::InitModeUseProjectileWeapon): eleven processes attached; for a living hero mode
// 25 is added only when the current mode differs, the ranged-attack zoom is reset when the component exists, and the
// projectile-weapon camera mode is pushed when a camera manager exists. A dead or missing hero skips all of it.
#include <cstdio>

static int g_added = 0;
struct CAInputProcess { void AddToInterface(int pi) { if (pi == 7) ++g_added; } };
struct CModes { unsigned long current; int adds; CModes() : current(0), adds(0) {} void AddMode(unsigned long m) { current = m; ++adds; } };
struct CRanged { int resets; CRanged() : resets(0) {} void ResetProjectileTargetingZoomParameter() { ++resets; } };
struct CCamera { int pushes; CCamera() : pushes(0) {} void PushProjectileWeaponMode() { ++pushes; } };
struct CHero { unsigned char dead; CModes* modes; CRanged* ranged; };
struct CPlayerModel {
    int PlayerInterface; CAInputProcess procs[11]; CCamera* camera;
    void Run(CHero* hero)
    {
        for (int i = 0; i < 11; ++i) procs[i].AddToInterface(PlayerInterface);
        if (hero && !(hero->dead & 1)) {
            if (hero->modes->current != 25) hero->modes->AddMode(25);
            if (hero->ranged) hero->ranged->ResetProjectileTargetingZoomParameter();
            if (camera) camera->PushProjectileWeaponMode();
        }
    }
};
int main()
{
    CModes modes; CRanged ranged; CCamera cam; CHero hero = { 0, &modes, &ranged };
    CPlayerModel m; m.PlayerInterface = 7; m.camera = &cam;
    m.Run(&hero);
    int ok = g_added == 11 && modes.adds == 1 && modes.current == 25 && ranged.resets == 1 && cam.pushes == 1;
    hero.ranged = 0; m.Run(&hero);                       // already in mode 25, no ranged component
    if (modes.adds != 1 || ranged.resets != 1 || cam.pushes != 2) ok = 0;
    hero.dead = 1; m.Run(&hero); m.Run(0);
    if (cam.pushes != 2 || g_added != 44) ok = 0;
    if (!ok) { std::printf("00635bf0_TEST FAIL\n"); return 1; }
    std::printf("00635bf0_TEST PASS\n");
    return 0;
}