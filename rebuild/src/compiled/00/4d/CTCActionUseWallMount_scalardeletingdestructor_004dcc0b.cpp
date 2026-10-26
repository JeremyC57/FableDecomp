#pragma optimize("s", on)
extern void __fastcall CTCActionUseWallMount_Destruct(void* mount);
void* __fastcall CTCActionUseWallMount_ScalarDeletingDestructor(void* mount, int, unsigned int flags)
{
    CTCActionUseWallMount_Destruct(mount);
    if (flags & 1)
        operator delete(mount);
    return mount;
}