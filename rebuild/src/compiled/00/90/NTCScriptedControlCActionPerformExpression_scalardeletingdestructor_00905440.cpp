#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CActionPerformExpression_Destruct(void* action);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActionPerformExpression_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(action) + 28);
    DestructSecondMember(static_cast<unsigned char*>(action) + 12);
    CActionPerformExpression_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}