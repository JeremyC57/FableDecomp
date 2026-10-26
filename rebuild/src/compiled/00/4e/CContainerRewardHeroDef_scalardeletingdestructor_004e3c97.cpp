#pragma optimize("s", on)
extern void __fastcall CContainerRewardHeroDef_Destruct(void* def);
void* __fastcall CContainerRewardHeroDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CContainerRewardHeroDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}