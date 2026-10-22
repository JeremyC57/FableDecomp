#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCCreatureExpression_Destruct(void* expression);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureExpression_CompositeScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(expression) + 12);
    CTCCreatureExpression_Destruct(expression);
    if (flags & 1)
        OperatorDelete(expression);
    return expression;
}