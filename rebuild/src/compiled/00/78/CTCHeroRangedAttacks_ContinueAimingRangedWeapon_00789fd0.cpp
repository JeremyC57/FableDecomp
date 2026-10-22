struct CTCHeroRangedAttacks {
    char pad[0x14];
    unsigned char FireCycleInProgress;
    unsigned char Aiming;
};

static bool __fastcall Helper(CTCHeroRangedAttacks* self);

void __fastcall ContinueAimingRangedWeapon(CTCHeroRangedAttacks* self)
{
    if (Helper(self)) {
        self->FireCycleInProgress = true;
        self->Aiming = true;
    }
}

#pragma optimize("", off)
static bool __fastcall Helper(CTCHeroRangedAttacks* self)
{
    return self->FireCycleInProgress != 0;
}
#pragma optimize("", on)