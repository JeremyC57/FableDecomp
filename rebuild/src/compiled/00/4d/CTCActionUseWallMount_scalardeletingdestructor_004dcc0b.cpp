#pragma optimize("s", on)
extern void __fastcall CTCActionUseWallMount_Destruct(void* mount);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseWallMount_ScalarDeletingDestructor(void* mount, int, unsigned int flags)
{
    CTCActionUseWallMount_Destruct(mount);
    if (flags & 1)
        OperatorDelete(mount);
    return mount;
}