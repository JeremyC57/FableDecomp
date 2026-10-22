extern unsigned char g_questScreenFlag;

struct CQuestManager {
    char pad[0x98];
    void* screenPtr;
};

bool __fastcall CQuestManager_IsBetweenQuestStartAndFinishScreens(const CQuestManager* manager)
{
    if (g_questScreenFlag)
        return manager->screenPtr != 0;
    return false;
}