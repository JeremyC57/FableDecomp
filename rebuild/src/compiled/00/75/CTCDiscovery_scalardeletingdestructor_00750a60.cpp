#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCDiscovery_Destruct(void* discovery);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDiscovery_CompositeScalarDeletingDestructor(void* discovery, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(discovery) + 13);
    CTCDiscovery_Destruct(discovery);
    if (flags & 1)
        OperatorDelete(discovery);
    return discovery;
}