struct CTCHeroRangedAttacks {
    char pad[0x14];
    unsigned char FireCycleInProgress;
    unsigned char Aiming;
};

static bool __fastcall Helper(CTCHeroRangedAttacks* attacks);

void __fastcall ContinueAimingRangedWeapon(CTCHeroRangedAttacks* attacks)
{
    if (Helper(attacks)) {
        attacks->FireCycleInProgress = true;
        attacks->Aiming = true;
    }
}

#pragma optimize("", off)
static bool __fastcall Helper(CTCHeroRangedAttacks* attacks)
{
    return attacks->FireCycleInProgress != 0;
}
#pragma optimize("", on)