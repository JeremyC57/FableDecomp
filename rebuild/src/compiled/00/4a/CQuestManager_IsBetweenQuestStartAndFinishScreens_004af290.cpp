// Leaf: ?IsBetweenQuestStartAndFinishScreens@CQuestManager@@QBE_NXZ
namespace NGlobalConsole { extern bool GEnableRegionLockingSaveSystem; }   // 0x01375741 ?GEnableRegionLockingSaveSystem@NGlobalConsole@@3_NA (Ego_r / FableWin / Ego_d publics; Codex p27 NAME_FIXES cross-reference); NF236: was unsigned char g_questScreenFlag

class CQuestManager { public:
    char pad[0x98];
    void* screenPtr;
    public: bool IsBetweenQuestStartAndFinishScreens() const;
};

bool CQuestManager::IsBetweenQuestStartAndFinishScreens() const
{
    const CQuestManager* manager = this;
    if (NGlobalConsole::GEnableRegionLockingSaveSystem)
        return manager->screenPtr != 0;
    return false;
}