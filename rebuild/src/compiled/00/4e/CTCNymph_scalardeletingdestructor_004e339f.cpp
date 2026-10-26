#pragma optimize("s", on)
extern void __fastcall CTCNymph_Destruct(void* nymph);
void* __fastcall CTCNymph_ScalarDeletingDestructor(void* nymph, int, unsigned int flags)
{
    CTCNymph_Destruct(nymph);
    if (flags & 1)
        operator delete(nymph);
    return nymph;
}