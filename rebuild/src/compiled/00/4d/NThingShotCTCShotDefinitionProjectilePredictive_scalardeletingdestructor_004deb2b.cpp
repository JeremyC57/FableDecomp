#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionProjectilePredictive_Destruct(void* predictive);
void* __fastcall CTCShotDefinitionProjectilePredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    CTCShotDefinitionProjectilePredictive_Destruct(predictive);
    if (flags & 1)
        operator delete(predictive);
    return predictive;
}