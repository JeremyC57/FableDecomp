#pragma optimize("t", on)
extern void __fastcall DestructMember_004aa840(void* member);
extern void __fastcall CActionFollowCreature_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionFollowCreature_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_004aa840(static_cast<unsigned char*>(action) + 12);
    CActionFollowCreature_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}