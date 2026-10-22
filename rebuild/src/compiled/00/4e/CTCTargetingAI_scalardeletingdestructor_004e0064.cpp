#pragma optimize("s", on)
extern void __fastcall CTCTargetingAI_Destruct(void* targetingAI);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTargetingAI_ScalarDeletingDestructor(void* targetingAI, int, unsigned int flags)
{
    CTCTargetingAI_Destruct(targetingAI);
    if (flags & 1)
        OperatorDelete(targetingAI);
    return targetingAI;
}