#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCOwnedEntity_Destruct(void* entity);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCOwnedEntity_CompositeScalarDeletingDestructor(void* entity, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(entity) + 13);
    CTCOwnedEntity_Destruct(entity);
    if (flags & 1)
        OperatorDelete(entity);
    return entity;
}