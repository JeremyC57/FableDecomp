#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CCreatureAction_TrollThrowProjectile_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCreatureAction_TrollThrowProjectile_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(action) + 276);
    CCreatureAction_TrollThrowProjectile_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}