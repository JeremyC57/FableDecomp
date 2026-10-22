#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseSpade_Destruct(void* spade);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseSpade_ScalarDeletingDestructor(void* spade, int, unsigned int flags)
{
    CTCCarriedActionUseSpade_Destruct(spade);
    if (flags & 1)
        OperatorDelete(spade);
    return spade;
}