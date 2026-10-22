#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionThrowPredictive_Destruct(void* predictive);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    CTCShotDefinitionThrowPredictive_Destruct(predictive);
    if (flags & 1)
        OperatorDelete(predictive);
    return predictive;
}