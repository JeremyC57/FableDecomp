#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CActionPlayCombatAnimation_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionPlayCombatAnimation_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(action) + 16);
    CActionPlayCombatAnimation_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}