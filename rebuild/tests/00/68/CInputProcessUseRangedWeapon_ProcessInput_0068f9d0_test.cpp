#include <cstdio>
// Behaviour test for CInputProcessUseRangedWeapon::ProcessInput (0x0068F9D0).
struct CTCWeapon { bool Projectile, Loaded, Analogue; };
struct CThing;
struct CTCCarrying { CThing* Primary; CThing* GetThingInPrimarySlot() { return Primary; } };
struct CRanged { int Cancelled, Reset; };
struct CPlayer { bool InRangedWeaponMode; int RemovedMode; void RemoveMode(long m) { RemovedMode = m; } };
struct CThing {
    bool Dead; CTCCarrying* Carrying; CTCWeapon* Weapon; CRanged* Ranged; bool MeleeCarried; CPlayer* Player;
    CPlayer* GetPlayer_004c7a30() { return Player; }
};
struct CInput {
    CPlayer* MyPlayer; CThing* Hero; int Forwarded; CThing* LastWeapon;
    void ProcessRangedAttackInput(int, int, CThing* w) { Forwarded++; LastWeapon = w; }
    void ProcessInput(int event, int processed)
    {
        CThing* hero = Hero;
        if (!MyPlayer->InRangedWeaponMode) {
            CTCCarrying* carrying = hero->Carrying;
            CThing* weapon = carrying->GetThingInPrimarySlot();
            CTCWeapon* w;
            if (weapon && !weapon->Dead && (w = weapon->Weapon) != 0 && w->Projectile) ProcessRangedAttackInput(event, processed, weapon);
            return;
        }
        if (!hero->MeleeCarried) return;
        CTCCarrying* carrying = hero->Carrying;
        CThing* weapon = carrying->GetThingInPrimarySlot();
        CTCWeapon* w;
        if (!weapon || weapon->Dead || (w = weapon->Weapon) == 0 || w->Loaded || !w->Analogue) {
            hero->Ranged->Cancelled++; hero->Ranged->Reset++;
            hero->GetPlayer_004c7a30()->RemoveMode(0x22);
        }
    }
};
static int Fail() { std::printf("0068f9d0_TEST FAIL\n"); return 1; }
int main()
{
    CTCWeapon bow = { true, false, true };
    CThing bowThing = { false, 0, &bow, 0, false, 0 };
    CTCCarrying carrying = { &bowThing };
    CRanged ranged = { 0, 0 };
    CPlayer player = { false, 0 };
    CThing hero = { false, &carrying, 0, &ranged, true, &player };
    CInput in = { &player, &hero, 0, 0 };
    in.ProcessInput(1, 2);
    if (in.Forwarded != 1 || in.LastWeapon != &bowThing) return Fail();
    bow.Projectile = false;
    in.ProcessInput(1, 2);
    if (in.Forwarded != 1) return Fail();
    player.InRangedWeaponMode = true;
    in.ProcessInput(1, 2);                // unloaded analogue-loading weapon: keep the mode
    if (ranged.Cancelled != 0 || player.RemovedMode != 0) return Fail();
    bow.Loaded = true;
    in.ProcessInput(1, 2);
    if (ranged.Cancelled != 1 || ranged.Reset != 1 || player.RemovedMode != 0x22) return Fail();
    hero.MeleeCarried = false; player.RemovedMode = 0;
    in.ProcessInput(1, 2);
    if (player.RemovedMode != 0) return Fail();
    std::printf("0068f9d0_TEST PASS\n");
    return 0;
}