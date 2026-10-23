#pragma optimize("s", on)
extern void __fastcall DestructMember_004b8940(void* member);
extern void __fastcall CTCThingOwner_Destruct(void* owner);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCThingOwner_CompositeScalarDeletingDestructor(void* owner, int, unsigned int flags)
{
    DestructMember_004b8940(static_cast<unsigned char*>(owner) + 12);
    CTCThingOwner_Destruct(owner);
    if (flags & 1)
        OperatorDelete(owner);
    return owner;
}