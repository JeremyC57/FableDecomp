class CUserProfileManager { public: char pad[0x33]; char flag; };
extern "C" void __fastcall UPM_Prep(CUserProfileManager* manager);
extern "C" void __fastcall UPM_Commit(CUserProfileManager* manager);

void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager)
{
    UPM_Prep(manager);
    if (manager->flag)
        UPM_Commit(manager);
}