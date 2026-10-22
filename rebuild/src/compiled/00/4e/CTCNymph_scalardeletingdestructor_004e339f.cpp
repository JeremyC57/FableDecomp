#pragma optimize("s", on)
extern void __fastcall CTCNymph_Destruct(void* nymph);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCNymph_ScalarDeletingDestructor(void* nymph, int, unsigned int flags)
{
    CTCNymph_Destruct(nymph);
    if (flags & 1)
        OperatorDelete(nymph);
    return nymph;
}