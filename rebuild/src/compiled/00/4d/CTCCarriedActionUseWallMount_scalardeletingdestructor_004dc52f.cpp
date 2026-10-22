#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseWallMount_Destruct(void* mount);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseWallMount_ScalarDeletingDestructor(void* mount, int, unsigned int flags)
{
    CTCCarriedActionUseWallMount_Destruct(mount);
    if (flags & 1)
        OperatorDelete(mount);
    return mount;
}