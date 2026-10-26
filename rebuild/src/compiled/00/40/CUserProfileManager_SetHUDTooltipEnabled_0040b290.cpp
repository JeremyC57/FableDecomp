class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;                 /* +0x0c */
    char pad1[0x19 - 0x0c - 1];
    char HUDTooltipEnabled;                   /* +0x19 */
    char pad2[0xd9 - 0x19 - 1];
    char HUDTooltipEnabledNewProfile;                   /* +0xd9 */
    void Helper2();
    void SetHUDTooltipEnabled(char a, char b);
protected:
    void ApplyValues();   // 0x00407370
};

void CUserProfileManager_SetHUDTooltipEnabled(CUserProfileManager* manager, char a, char b);

void CUserProfileManager::SetHUDTooltipEnabled(char a, char b)
{
    if (this->UseNewProfileValues)
        this->HUDTooltipEnabledNewProfile = a;
    this->HUDTooltipEnabled = a;
    ApplyValues();
    if (b)
        this->Helper2();
}