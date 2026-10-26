#pragma optimize("s", on)
extern void __fastcall CTCTargetingAI_Destruct(void* targetingAI);
void* __fastcall CTCTargetingAI_ScalarDeletingDestructor(void* targetingAI, int, unsigned int flags)
{
    CTCTargetingAI_Destruct(targetingAI);
    if (flags & 1)
        operator delete(targetingAI);
    return targetingAI;
}