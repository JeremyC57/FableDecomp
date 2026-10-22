#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCGossip_Destruct(void* gossip);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGossip_CompositeScalarDeletingDestructor(void* gossip, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(gossip) + 12);
    CTCGossip_Destruct(gossip);
    if (flags & 1)
        OperatorDelete(gossip);
    return gossip;
}