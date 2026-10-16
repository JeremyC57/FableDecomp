// CTCHeroRangedAttacks::ResetProjectileWeaponMode() -- leave the projectile-weapon player mode (0x19) and reset the first-person zoom index; while a fire cycle is in progress, make sure the aiming mode 0x22 is on unless mode 0x22 or 0x2E is already active
// Leaf: ResetProjectileWeaponMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789B30, length 97 bytes (+15 int3 padding).
// Callees: CThing::GetPlayer() @ 0x004C7A30 (four times), CPlayer::RemoveMode(EPlayerMode) @ 0x00634190, CPlayer::HasMode
// (EPlayerMode) @ 0x00634160, CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0.
// Shape: each call pushes its mode before the GetPlayer() object expression; the -1 store is scheduled between that push and
// the GetPlayer call.
// Layout: Ego_r.pdb CTCHeroRangedAttacks (engine/CTCHeroRangedAttacks.h, same offsets in retail): FireCycleInProgress +0x14,
// ProjectileWeaponFirstPersonZoomModeIndex +0x44.
enum EPlayerMode { PLAYER_MODE_NONE };
class CPlayer {
public:
    void RemoveMode(EPlayerMode mode);              // 0x00634190
    bool HasMode(EPlayerMode mode);                 // 0x00634160
    void AddMode(EPlayerMode mode, bool b);         // 0x006345C0
};
class CThing {
public:
    CPlayer* GetPlayer();                           // 0x004C7A30
};
class CThingCreatureBase;
class CTCHeroRangedAttacks {
public:
    virtual ~CTCHeroRangedAttacks();
    CThing*             ParentThing;                // +0x04
    unsigned char       _pad08[0x4];
    CThingCreatureBase* PParentCreature;            // +0x0c
    void*               PHeroCombatDef;             // +0x10
    bool                FireCycleInProgress;        // +0x14
    bool                Aiming;                     // +0x15
    bool                FireQueued;                 // +0x16
    bool                Loading;                    // +0x17
    unsigned char       _pad18[0x44 - 0x18];
    long                ProjectileWeaponFirstPersonZoomModeIndex;   // +0x44
    void ResetProjectileWeaponMode();
};

void CTCHeroRangedAttacks::ResetProjectileWeaponMode()
{
    ProjectileWeaponFirstPersonZoomModeIndex = -1;
    ParentThing->GetPlayer()->RemoveMode((EPlayerMode)0x19);
    if (FireCycleInProgress) {
        if (!ParentThing->GetPlayer()->HasMode((EPlayerMode)0x22) &&
            !ParentThing->GetPlayer()->HasMode((EPlayerMode)0x2e))
            ParentThing->GetPlayer()->AddMode((EPlayerMode)0x22, false);
    }
}
