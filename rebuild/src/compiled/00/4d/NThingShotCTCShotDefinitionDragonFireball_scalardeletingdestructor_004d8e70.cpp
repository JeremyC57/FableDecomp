#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionDragonFireball_Destruct(void* fireball);
void* __fastcall CTCShotDefinitionDragonFireball_ScalarDeletingDestructor(void* fireball, int, unsigned int flags)
{
    CTCShotDefinitionDragonFireball_Destruct(fireball);
    if (flags & 1)
        operator delete(fireball);
    return fireball;
}