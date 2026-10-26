#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseWallMount_Destruct(void* mount);
void* __fastcall CTCCarriedActionUseWallMount_ScalarDeletingDestructor(void* mount, int, unsigned int flags)
{
    CTCCarriedActionUseWallMount_Destruct(mount);
    if (flags & 1)
        operator delete(mount);
    return mount;
}