class CUserProfileManager { public:
    char pad_c[0xc];
    char UseNewProfileValues;
    char SubtitlingEnabled;
    char pad_e[0xcd - 0xe];
    char SubtitlingEnabledNewProfile;
    void ApplyCamera();
    void NotifyReset();
    void SetSubtitlingEnabled(char a1, char a2);
};

void CUserProfileManager_SetSubtitlingEnabled(CUserProfileManager* manager, char a1, char a2);

void CUserProfileManager::SetSubtitlingEnabled(char a1, char a2)
{
    if (this->UseNewProfileValues)
        this->SubtitlingEnabledNewProfile = a1;
    this->SubtitlingEnabled = a1;
    this->ApplyCamera();
    if (a2)
        this->NotifyReset();
}