#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCCreatureItemLevitationNymph_Destruct(void* nymph);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureItemLevitationNymph_CompositeScalarDeletingDestructor(void* nymph, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(nymph) + 40);
    CTCCreatureItemLevitationNymph_Destruct(nymph);
    if (flags & 1)
        OperatorDelete(nymph);
    return nymph;
}