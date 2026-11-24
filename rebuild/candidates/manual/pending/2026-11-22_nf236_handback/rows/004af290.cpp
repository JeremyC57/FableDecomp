namespace NGlobalConsole { extern bool GEnableRegionLockingSaveSystem; }   // 0x01375741 ?GEnableRegionLockingSaveSystem@NGlobalConsole@@3_NA (Ego_r / FableWin / Ego_d publics; Codex p27 NAME_FIXES cross-reference); NF236: was unsigned char g_questScreenFlag

class CQuestManager { public:
    char pad[0x98];
    void* screenPtr;
};

bool __fastcall CQuestManager_IsBetweenQuestStartAndFinishScreens(const CQuestManager* manager)
{
    if (NGlobalConsole::GEnableRegionLockingSaveSystem)
        return manager->screenPtr != 0;
    return false;
}