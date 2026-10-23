#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_0066a1c0(void* member);
extern void __fastcall DestructSecondMember_004aa840(void* member);
extern void __fastcall CActionFireProjectileWeapon_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionFireProjectileWeapon_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_0066a1c0(static_cast<unsigned char*>(action) + 24);
    DestructSecondMember_004aa840(static_cast<unsigned char*>(action) + 12);
    CActionFireProjectileWeapon_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}