#pragma optimize("s", on)
extern void __fastcall CTCGeneratesExperienceOnKilling_Destruct(void* killing);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGeneratesExperienceOnKilling_ScalarDeletingDestructor(void* killing, int, unsigned int flags)
{
    CTCGeneratesExperienceOnKilling_Destruct(killing);
    if (flags & 1)
        OperatorDelete(killing);
    return killing;
}