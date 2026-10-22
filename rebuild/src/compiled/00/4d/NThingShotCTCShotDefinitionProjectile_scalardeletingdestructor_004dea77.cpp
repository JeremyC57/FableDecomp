#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionProjectile_Destruct(void* projectile);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionProjectile_ScalarDeletingDestructor(void* projectile, int, unsigned int flags)
{
    CTCShotDefinitionProjectile_Destruct(projectile);
    if (flags & 1)
        OperatorDelete(projectile);
    return projectile;
}