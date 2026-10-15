// Behaviour test for CUserProfileManager::SetAntialiasing (resolution setter, 0x0040B690).
#include <cstdio>

struct CUserProfileManager {
    bool UseNewProfileValues;
    unsigned long Resolution;
    unsigned long ResolutionNewProfile;
    int applied, saved;
    void ApplyValues() { ++applied; }
    void Save() { ++saved; }
    void SetAntialiasing(unsigned long value, bool save);
};

void CUserProfileManager::SetAntialiasing(unsigned long value, bool save)
{
    if (UseNewProfileValues)
        ResolutionNewProfile = value;
    Resolution = value;
    ApplyValues();
    if (save)
        Save();
}

int main()
{
    CUserProfileManager m;
    m.UseNewProfileValues = false; m.Resolution = 1; m.ResolutionNewProfile = 1; m.applied = 0; m.saved = 0;
    m.SetAntialiasing(5, false);
    if (m.Resolution != 5 || m.ResolutionNewProfile != 1 || m.applied != 1 || m.saved != 0) { std::printf("0040b690_TEST FAIL\n"); return 1; }
    m.UseNewProfileValues = true;
    m.SetAntialiasing(7, true);
    if (m.Resolution != 7 || m.ResolutionNewProfile != 7 || m.applied != 2 || m.saved != 1) { std::printf("0040b690_TEST FAIL\n"); return 1; }
    std::printf("0040b690_TEST PASS\n");
    return 0;
}
