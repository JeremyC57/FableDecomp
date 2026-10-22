#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCCreatedEntity_Destruct(void* entity);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatedEntity_CompositeScalarDeletingDestructor(void* entity, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(entity) + 12);
    CTCCreatedEntity_Destruct(entity);
    if (flags & 1)
        OperatorDelete(entity);
    return entity;
}