#pragma optimize("s", on)
extern void __fastcall CTCActionUseStartExtrapolationMorph_Destruct(void* morph);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseStartExtrapolationMorph_ScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    CTCActionUseStartExtrapolationMorph_Destruct(morph);
    if (flags & 1)
        OperatorDelete(morph);
    return morph;
}