#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_004aa840(void* member);
extern void __fastcall DestructSecondMember_00a01c10(void* member);
extern void __fastcall CActionTalkedTo_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionTalkedTo_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_004aa840(static_cast<unsigned char*>(action) + 20);
    DestructSecondMember_00a01c10(static_cast<unsigned char*>(action) + 12);
    CActionTalkedTo_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}