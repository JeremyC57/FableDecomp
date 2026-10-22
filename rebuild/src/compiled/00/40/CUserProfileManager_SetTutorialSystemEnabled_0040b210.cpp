struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;      // +0xc
    char pad1[0xa];
    char TutorialSystemEnabled;     // +0x17
    char pad2[0xbf];
    char TutorialSystemEnabledNewProfile;     // +0xd7
};

void __fastcall CUserProfileManager_Helper1(CUserProfileManager* manager);
void __fastcall CUserProfileManager_Helper2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetTutorialSystemEnabled(CUserProfileManager* manager, int edx_dummy, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->TutorialSystemEnabledNewProfile = p1;
    manager->TutorialSystemEnabled = p1;
    CUserProfileManager_Helper1(manager);
    if (p2)
        CUserProfileManager_Helper2(manager);
}