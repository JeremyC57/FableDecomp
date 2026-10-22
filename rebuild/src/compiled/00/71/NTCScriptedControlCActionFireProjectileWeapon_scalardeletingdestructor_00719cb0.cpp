#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CActionFireProjectileWeapon_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionFireProjectileWeapon_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(action) + 24);
    DestructSecondMember(static_cast<unsigned char*>(action) + 12);
    CActionFireProjectileWeapon_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}