#pragma optimize("s", on)
extern void __fastcall CTCInterestingToVillagers_Destruct(void* villagers);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInterestingToVillagers_ScalarDeletingDestructor(void* villagers, int, unsigned int flags)
{
    CTCInterestingToVillagers_Destruct(villagers);
    if (flags & 1)
        OperatorDelete(villagers);
    return villagers;
}