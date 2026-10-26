class CUserProfileManager { public:
    char pad00[0x0c];
    unsigned char UseNewProfileValues;
    char pad0d[0x32 - 0x0d];
    unsigned char ShowingBuddyNames;
    char pad33[0xf1 - 0x33];
    unsigned char ShowingBuddyNamesNewProfile;
};

extern void __fastcall UPM_MethodA(CUserProfileManager* manager);
extern void __fastcall UPM_MethodB(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetShowingBuddyNames(CUserProfileManager* manager, int edx, char p1, char p2)
{
    (void)edx;
    if (manager->UseNewProfileValues)
        manager->ShowingBuddyNamesNewProfile = (unsigned char)p1;
    manager->ShowingBuddyNames = (unsigned char)p1;
    UPM_MethodA(manager);
    if (p2)
        UPM_MethodB(manager);
}