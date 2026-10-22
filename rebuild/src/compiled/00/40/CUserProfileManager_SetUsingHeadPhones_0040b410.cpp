struct CUserProfileManager {
    char pad0[0xc];
    char flag0c;
    char pad_d[0x30-0xd];
    char UsingHeadPhones;
    char pad_31[0xf0-0x31];
    char UsingHeadPhonesNewProfile;
};

extern void __fastcall CUPM_Notify1(CUserProfileManager* manager);
extern void __fastcall CUPM_Notify2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetUsingHeadPhones(CUserProfileManager* manager, void* edx, char p1, char p2)
{
    (void)edx;
    if (manager->flag0c)
        manager->UsingHeadPhonesNewProfile = p1;
    manager->UsingHeadPhones = p1;
    CUPM_Notify1(manager);
    if (p2)
        CUPM_Notify2(manager);
}