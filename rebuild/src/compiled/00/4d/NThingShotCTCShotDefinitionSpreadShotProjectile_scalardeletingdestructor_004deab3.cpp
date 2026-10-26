#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionSpreadShotProjectile_Destruct(void* projectile);
void* __fastcall CTCShotDefinitionSpreadShotProjectile_ScalarDeletingDestructor(void* projectile, int, unsigned int flags)
{
    CTCShotDefinitionSpreadShotProjectile_Destruct(projectile);
    if (flags & 1)
        operator delete(projectile);
    return projectile;
}