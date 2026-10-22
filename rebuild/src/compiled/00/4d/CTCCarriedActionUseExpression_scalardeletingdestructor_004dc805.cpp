#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseExpression_Destruct(void* expression);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseExpression_ScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    CTCCarriedActionUseExpression_Destruct(expression);
    if (flags & 1)
        OperatorDelete(expression);
    return expression;
}