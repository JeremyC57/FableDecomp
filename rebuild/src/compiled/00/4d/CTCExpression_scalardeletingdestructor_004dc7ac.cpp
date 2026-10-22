#pragma optimize("s", on)
extern void __fastcall CTCExpression_Destruct(void* expression);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCExpression_ScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    CTCExpression_Destruct(expression);
    if (flags & 1)
        OperatorDelete(expression);
    return expression;
}