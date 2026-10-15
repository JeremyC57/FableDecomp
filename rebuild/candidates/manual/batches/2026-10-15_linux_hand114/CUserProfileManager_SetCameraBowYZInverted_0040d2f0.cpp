// CUserProfileManager::SetCameraBowYZInverted(bool, bool) -- set both bow-camera Y/Z inversion flags (first-person +0x12, PC +0x14) through their own setters
// Retail: Fable.exe @ 0x0040D2F0, 89 bytes (+ int3 padding), `ret 8`.
// Callees: CUserProfileManager::ApplyValues @ 0x00407370, CUserProfileManager::Save @ 0x0040A740 (each twice).
// Layout (Ego_r.pdb; the new-profile block sits 4 bytes below the PDB offsets in retail): UseNewProfileValues +0x0c,
// CameraFPBowYZInverted +0x12, CameraPCBowYZInverted +0x14, their NewProfile copies +0xd2 / +0xd4.
// Shape: two inlined single-flag setters (the 0x0040B690 setter shape: new-profile copy under the flag, the live
// value, ApplyValues, optional Save); the bool argument is held in bl across all four calls, the save flag is
// re-read from its stack slot at each test.
struct CUserProfileManager {
    void* _vtables[2];                              // +0x00 / +0x04
    void* ProfileName;                              // +0x08 (CWideString)
    bool UseNewProfileValues;                       // +0x0c
    unsigned char _pad0d[0x12 - 0x0d];
    bool CameraFPBowYZInverted;                     // +0x12
    bool CameraPCBowXYInverted;                     // +0x13
    bool CameraPCBowYZInverted;                     // +0x14
    unsigned char _pad15[0xd2 - 0x15];
    bool CameraFPBowYZInvertedNewProfile;           // +0xd2
    bool CameraPCBowXYInvertedNewProfile;           // +0xd3
    bool CameraPCBowYZInvertedNewProfile;           // +0xd4
    void ApplyValues();                             // 0x00407370
    void Save();                                    // 0x0040A740
    void SetCameraFPBowYZInverted(bool inverted, bool save)
    {
        if (UseNewProfileValues)
            CameraFPBowYZInvertedNewProfile = inverted;
        CameraFPBowYZInverted = inverted;
        ApplyValues();
        if (save)
            Save();
    }
    void SetCameraPCBowYZInverted(bool inverted, bool save)
    {
        if (UseNewProfileValues)
            CameraPCBowYZInvertedNewProfile = inverted;
        CameraPCBowYZInverted = inverted;
        ApplyValues();
        if (save)
            Save();
    }
    void SetCameraBowYZInverted(bool inverted, bool save);
};

void CUserProfileManager::SetCameraBowYZInverted(bool inverted, bool save)
{
    SetCameraFPBowYZInverted(inverted, save);
    SetCameraPCBowYZInverted(inverted, save);
}
