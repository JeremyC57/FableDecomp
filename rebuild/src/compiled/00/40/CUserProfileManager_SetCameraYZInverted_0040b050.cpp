class CUserProfileManager { public:
    char pad_0[0xc];
    char m_flag;      /* +0xc */
    char pad_d[3];    /* +0xd..0xf */
    char CameraYZInverted;   /* +0x10 */
    char pad_11[0xbf];/* +0x11..0xcf */
    char CameraYZInvertedNewProfile;   /* +0xd0 */
    void Apply();
    void SetCameraYZInverted(char value, char doApply);
protected:
    void ApplyValues();   // 0x00407370
};

void CUserProfileManager::SetCameraYZInverted(char value, char doApply)
{
    if (this->m_flag != 0)
        this->CameraYZInvertedNewProfile = value;
    this->CameraYZInverted = value;
    ApplyValues();
    if (doApply != 0)
        this->Apply();
}