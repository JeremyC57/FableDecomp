#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseActivateQuest_Destruct(void* quest);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseActivateQuest_ScalarDeletingDestructor(void* quest, int, unsigned int flags)
{
    CTCCarriedActionUseActivateQuest_Destruct(quest);
    if (flags & 1)
        OperatorDelete(quest);
    return quest;
}