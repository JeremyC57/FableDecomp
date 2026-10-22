#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionSpreadShotProjectile_Destruct(void* projectile);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionSpreadShotProjectile_ScalarDeletingDestructor(void* projectile, int, unsigned int flags)
{
    CTCShotDefinitionSpreadShotProjectile_Destruct(projectile);
    if (flags & 1)
        OperatorDelete(projectile);
    return projectile;
}