#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseExpression_Destruct(void* expression);
void* __fastcall CTCCarriedActionUseExpression_ScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    CTCCarriedActionUseExpression_Destruct(expression);
    if (flags & 1)
        operator delete(expression);
    return expression;
}