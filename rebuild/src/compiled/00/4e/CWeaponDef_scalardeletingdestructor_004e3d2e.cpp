#pragma optimize("s", on)
extern void __fastcall CWeaponDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CWeaponDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CWeaponDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}