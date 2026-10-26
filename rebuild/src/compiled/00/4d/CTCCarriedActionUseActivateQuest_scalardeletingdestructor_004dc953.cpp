#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseActivateQuest_Destruct(void* quest);
void* __fastcall CTCCarriedActionUseActivateQuest_ScalarDeletingDestructor(void* quest, int, unsigned int flags)
{
    CTCCarriedActionUseActivateQuest_Destruct(quest);
    if (flags & 1)
        operator delete(quest);
    return quest;
}