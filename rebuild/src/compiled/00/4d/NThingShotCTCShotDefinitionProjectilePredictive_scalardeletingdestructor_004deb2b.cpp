#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionProjectilePredictive_Destruct(void* predictive);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionProjectilePredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    CTCShotDefinitionProjectilePredictive_Destruct(predictive);
    if (flags & 1)
        OperatorDelete(predictive);
    return predictive;
}