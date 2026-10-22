#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCWallMount_Destruct(void* mount);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCWallMount_CompositeScalarDeletingDestructor(void* mount, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mount) + 16);
    CTCWallMount_Destruct(mount);
    if (flags & 1)
        OperatorDelete(mount);
    return mount;
}