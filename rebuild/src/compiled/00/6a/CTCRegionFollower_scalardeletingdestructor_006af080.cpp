#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCRegionFollower_Destruct(void* follower);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCRegionFollower_CompositeScalarDeletingDestructor(void* follower, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(follower) + 12);
    CTCRegionFollower_Destruct(follower);
    if (flags & 1)
        OperatorDelete(follower);
    return follower;
}