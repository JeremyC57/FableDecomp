#pragma optimize("s",on)
struct EWeaponClass;

struct AllocT {
    EWeaponClass* construct_00535fe4(EWeaponClass* p, char* local);
};

EWeaponClass* __fastcall _Umove_EWeaponClass(AllocT* self, int edx_unused, EWeaponClass* p)
{
    char local;
    return self->construct_00535fe4(p, &local);
}