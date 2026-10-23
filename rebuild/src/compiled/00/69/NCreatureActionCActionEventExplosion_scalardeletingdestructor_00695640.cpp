#pragma optimize("t", on)
extern void __fastcall DestructMember_0099eae0(void* member);
extern void __fastcall CActionEventExplosion_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionEventExplosion_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(action) + 20);
    CActionEventExplosion_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}