#pragma optimize("s", on)
extern void __fastcall CTCFireRangedWeaponOnLeavingStateGroup_Destruct(void* group);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCFireRangedWeaponOnLeavingStateGroup_ScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    CTCFireRangedWeaponOnLeavingStateGroup_Destruct(group);
    if (flags & 1)
        OperatorDelete(group);
    return group;
}