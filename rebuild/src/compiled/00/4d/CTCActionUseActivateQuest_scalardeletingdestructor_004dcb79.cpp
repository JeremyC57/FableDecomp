#pragma optimize("s", on)
extern void __fastcall CTCActionUseActivateQuest_Destruct(void* quest);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseActivateQuest_ScalarDeletingDestructor(void* quest, int, unsigned int flags)
{
    CTCActionUseActivateQuest_Destruct(quest);
    if (flags & 1)
        OperatorDelete(quest);
    return quest;
}