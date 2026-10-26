#pragma optimize("t", on)
extern void __fastcall DestructMember_0099eae0(void* member);
extern void __fastcall CActionPlayAnimation_Destruct(void* action);
void* __fastcall CActionPlayAnimation_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(action) + 16);
    CActionPlayAnimation_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}