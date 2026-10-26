#pragma optimize("t", on)
extern void __fastcall DestructMember_0048c5c0(void* member);
extern void __fastcall CActionDoCreatureAction_Destruct(void* action);
void* __fastcall CActionDoCreatureAction_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0048c5c0(static_cast<unsigned char*>(action) + 20);
    CActionDoCreatureAction_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}