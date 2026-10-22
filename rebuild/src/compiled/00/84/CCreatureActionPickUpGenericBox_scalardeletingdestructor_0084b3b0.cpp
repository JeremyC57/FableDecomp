#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CCreatureAction_PickUpGenericBox_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCreatureAction_PickUpGenericBox_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(action) + 168);
    CCreatureAction_PickUpGenericBox_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}