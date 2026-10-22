struct CProjectileWeapon {
    void* vftable;
};

void __fastcall CProjectileWeapon_dtor(CProjectileWeapon* weapon);
void operator delete(void*);

void* __fastcall CProjectileWeapon_scalar_deleting_destructor(CProjectileWeapon* weapon, int edx_unused, unsigned int flags)
{
    CProjectileWeapon_dtor(weapon);
    if (flags & 1)
        operator delete(weapon);
    return weapon;
}