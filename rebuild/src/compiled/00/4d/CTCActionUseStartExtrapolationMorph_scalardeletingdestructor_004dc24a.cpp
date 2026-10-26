#pragma optimize("s", on)
extern void __fastcall CTCActionUseStartExtrapolationMorph_Destruct(void* morph);
void* __fastcall CTCActionUseStartExtrapolationMorph_ScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    CTCActionUseStartExtrapolationMorph_Destruct(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}