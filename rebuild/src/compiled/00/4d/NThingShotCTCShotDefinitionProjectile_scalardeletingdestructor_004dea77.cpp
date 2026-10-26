#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionProjectile_Destruct(void* projectile);
void* __fastcall CTCShotDefinitionProjectile_ScalarDeletingDestructor(void* projectile, int, unsigned int flags)
{
    CTCShotDefinitionProjectile_Destruct(projectile);
    if (flags & 1)
        operator delete(projectile);
    return projectile;
}