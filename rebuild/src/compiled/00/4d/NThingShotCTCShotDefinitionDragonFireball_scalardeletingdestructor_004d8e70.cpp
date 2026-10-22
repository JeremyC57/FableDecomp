#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionDragonFireball_Destruct(void* fireball);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionDragonFireball_ScalarDeletingDestructor(void* fireball, int, unsigned int flags)
{
    CTCShotDefinitionDragonFireball_Destruct(fireball);
    if (flags & 1)
        OperatorDelete(fireball);
    return fireball;
}