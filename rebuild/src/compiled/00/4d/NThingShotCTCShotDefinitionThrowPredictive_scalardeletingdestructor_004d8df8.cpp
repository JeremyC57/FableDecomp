#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionThrowPredictive_Destruct(void* predictive);
void* __fastcall CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    CTCShotDefinitionThrowPredictive_Destruct(predictive);
    if (flags & 1)
        operator delete(predictive);
    return predictive;
}