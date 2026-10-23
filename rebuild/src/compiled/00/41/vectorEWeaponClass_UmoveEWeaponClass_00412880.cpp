struct EWeaponClass;

struct AllocT {
    EWeaponClass* construct_004122d0(EWeaponClass* p, char* local);
};

EWeaponClass* __fastcall vector_Umove(AllocT* self, int edx_unused, EWeaponClass* p)
{
    char local = 0;
    return self->construct_004122d0(p, &local);
}