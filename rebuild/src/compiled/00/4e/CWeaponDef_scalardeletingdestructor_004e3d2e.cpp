#pragma optimize("s", on)
extern void __fastcall CWeaponDef_Destruct(void* def);
void* __fastcall CWeaponDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CWeaponDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}