#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCTargetingSpirit_Destruct(void* spirit);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTargetingSpirit_CompositeScalarDeletingDestructor(void* spirit, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(spirit) + 60);
    CTCTargetingSpirit_Destruct(spirit);
    if (flags & 1)
        OperatorDelete(spirit);
    return spirit;
}