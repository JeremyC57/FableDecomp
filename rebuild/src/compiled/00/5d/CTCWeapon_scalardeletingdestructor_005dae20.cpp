struct CTCWeapon {
    void* vftable;
};

void __fastcall CTCWeapon_dtor(CTCWeapon* weapon);
void operator delete(void*);

void* __fastcall CTCWeapon_scalar_deleting_destructor(CTCWeapon* weapon, int edx_unused, unsigned int flags)
{
    CTCWeapon_dtor(weapon);
    if (flags & 1)
        operator delete(weapon);
    return weapon;
}