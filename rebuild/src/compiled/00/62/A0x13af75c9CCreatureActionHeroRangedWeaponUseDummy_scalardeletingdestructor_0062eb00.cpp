struct CCreatureAction_HeroRangedWeaponUseDummy {
    void* vftable;
};

void __fastcall CCreatureAction_HeroRangedWeaponUseDummy_dtor(CCreatureAction_HeroRangedWeaponUseDummy* action);
void operator delete(void*);

void* __fastcall CCreatureAction_HeroRangedWeaponUseDummy_scalar_deleting_destructor(CCreatureAction_HeroRangedWeaponUseDummy* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_HeroRangedWeaponUseDummy_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}