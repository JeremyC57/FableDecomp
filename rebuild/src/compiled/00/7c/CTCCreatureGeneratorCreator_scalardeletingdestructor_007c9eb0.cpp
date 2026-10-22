#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCCreatureGeneratorCreator_Destruct(void* creator);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureGeneratorCreator_CompositeScalarDeletingDestructor(void* creator, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(creator) + 17);
    CTCCreatureGeneratorCreator_Destruct(creator);
    if (flags & 1)
        OperatorDelete(creator);
    return creator;
}