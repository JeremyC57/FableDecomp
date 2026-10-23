#pragma optimize("t", on)
extern void __fastcall DestructMember_0048c5c0(void* member);
extern void __fastcall CCreatureAction_CastAssassinRush_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCreatureAction_CastAssassinRush_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0048c5c0(static_cast<unsigned char*>(action) + 300);
    CCreatureAction_CastAssassinRush_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}