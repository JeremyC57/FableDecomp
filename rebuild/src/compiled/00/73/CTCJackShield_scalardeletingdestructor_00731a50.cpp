#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCJackShield_Destruct(void* shield);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCJackShield_CompositeScalarDeletingDestructor(void* shield, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(shield) + 12);
    CTCJackShield_Destruct(shield);
    if (flags & 1)
        OperatorDelete(shield);
    return shield;
}