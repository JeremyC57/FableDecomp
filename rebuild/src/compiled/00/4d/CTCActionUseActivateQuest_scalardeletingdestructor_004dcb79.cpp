#pragma optimize("s", on)
extern void __fastcall CTCActionUseActivateQuest_Destruct(void* quest);
void* __fastcall CTCActionUseActivateQuest_ScalarDeletingDestructor(void* quest, int, unsigned int flags)
{
    CTCActionUseActivateQuest_Destruct(quest);
    if (flags & 1)
        operator delete(quest);
    return quest;
}