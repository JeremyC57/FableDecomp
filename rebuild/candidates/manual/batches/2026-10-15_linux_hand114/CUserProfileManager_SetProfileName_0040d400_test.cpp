// Behaviour test for CUserProfileManager::SetProfileName (0x0040D400).
#include <cstdio>

struct CWideString {
    const wchar_t* m_text;
    CWideString& operator=(const CWideString& other) { m_text = other.m_text; return *this; }
};

struct CUserProfileManager {
    CWideString ProfileName;
    int loads, saves;
    void Load() { ++loads; }
    void Save() { ++saves; }
    void SetProfileName(const CWideString& profileName, bool load);
};

void CUserProfileManager::SetProfileName(const CWideString& profileName, bool load)
{
    ProfileName = profileName;
    if (load)
        Load();
    else
        Save();
}

int main()
{
    static const wchar_t name[] = L"Hero";
    CWideString n; n.m_text = name;
    CUserProfileManager m; m.ProfileName.m_text = 0; m.loads = m.saves = 0;
    m.SetProfileName(n, true);
    if (m.ProfileName.m_text != name || m.loads != 1 || m.saves != 0) { std::printf("0040d400_TEST FAIL\n"); return 1; }
    m.SetProfileName(n, false);
    if (m.loads != 1 || m.saves != 1) { std::printf("0040d400_TEST FAIL\n"); return 1; }
    std::printf("0040d400_TEST PASS\n");
    return 0;
}
