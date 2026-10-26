#pragma optimize("s", on)
extern void __fastcall CTCInterestingToVillagers_Destruct(void* villagers);
void* __fastcall CTCInterestingToVillagers_ScalarDeletingDestructor(void* villagers, int, unsigned int flags)
{
    CTCInterestingToVillagers_Destruct(villagers);
    if (flags & 1)
        operator delete(villagers);
    return villagers;
}