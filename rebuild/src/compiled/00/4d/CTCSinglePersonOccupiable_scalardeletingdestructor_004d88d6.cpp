#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCSinglePersonOccupiable_Destruct(void* occupiable);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSinglePersonOccupiable_CompositeScalarDeletingDestructor(void* occupiable, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(occupiable) + 12);
    CTCSinglePersonOccupiable_Destruct(occupiable);
    if (flags & 1)
        OperatorDelete(occupiable);
    return occupiable;
}