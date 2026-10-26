#pragma optimize("s", on)
extern void __fastcall CTCExpression_Destruct(void* expression);
void* __fastcall CTCExpression_ScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    CTCExpression_Destruct(expression);
    if (flags & 1)
        operator delete(expression);
    return expression;
}