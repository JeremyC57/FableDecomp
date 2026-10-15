// Behaviour test for CUserProfileManager::SetCameraBowYZInverted (0x0040D2F0).
#include <cstdio>

struct CUserProfileManager {
    bool UseNewProfileValues;
    bool CameraFPBowYZInverted, CameraPCBowYZInverted;
    bool CameraFPBowYZInvertedNewProfile, CameraPCBowYZInvertedNewProfile;
    int applied, saved;
    void ApplyValues() { ++applied; }
    void Save() { ++saved; }
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

int main()
{
    CUserProfileManager m;
    m.UseNewProfileValues = false;
    m.CameraFPBowYZInverted = m.CameraPCBowYZInverted = false;
    m.CameraFPBowYZInvertedNewProfile = m.CameraPCBowYZInvertedNewProfile = false;
    m.applied = m.saved = 0;
    m.SetCameraBowYZInverted(true, false);
    if (!m.CameraFPBowYZInverted || !m.CameraPCBowYZInverted || m.CameraFPBowYZInvertedNewProfile
        || m.CameraPCBowYZInvertedNewProfile || m.applied != 2 || m.saved != 0) { std::printf("0040d2f0_TEST FAIL\n"); return 1; }
    m.UseNewProfileValues = true;
    m.SetCameraBowYZInverted(true, true);
    if (!m.CameraFPBowYZInvertedNewProfile || !m.CameraPCBowYZInvertedNewProfile || m.applied != 4 || m.saved != 2) { std::printf("0040d2f0_TEST FAIL\n"); return 1; }
    std::printf("0040d2f0_TEST PASS\n");
    return 0;
}
