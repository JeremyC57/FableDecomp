#pragma optimize("s", on)
extern void __fastcall DestructMember_004406d0(void* member);
extern void __fastcall CTCTargetingSpirit_Destruct(void* spirit);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTargetingSpirit_CompositeScalarDeletingDestructor(void* spirit, int, unsigned int flags)
{
    DestructMember_004406d0(static_cast<unsigned char*>(spirit) + 60);
    CTCTargetingSpirit_Destruct(spirit);
    if (flags & 1)
        OperatorDelete(spirit);
    return spirit;
}