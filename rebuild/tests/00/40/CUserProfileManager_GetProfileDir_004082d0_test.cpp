#include <cstdio>
#include <cstring>

// Model of 0x004082D0 CUserProfileManager::GetProfileDir: save ProfileName, set it to `name`, resolve
// GetPathNameFromFileName(L"Profile.bin", create) (0x00408120), restore the saved name, return the path.
class CWideString {
public:
    char Text[96];
    explicit CWideString(const char* text) { std::strcpy(Text, text); }
    CWideString(const CWideString& other) { std::strcpy(Text, other.Text); }
    const CWideString& operator=(const CWideString& other) { std::strcpy(Text, other.Text); return *this; }
};
static CWideString g_ProfileFileName("Profile.bin");
class CUserProfileManager {
public:
    long _unknown04;
    CWideString ProfileName;
    mutable int LastCreate;
    CUserProfileManager() : _unknown04(0), ProfileName("Hero"), LastCreate(-1) { }
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const
    {
        LastCreate = create ? 1 : 0;
        CWideString path("Profiles\\");
        std::strcat(path.Text, ProfileName.Text);
        std::strcat(path.Text, "\\");
        std::strcat(path.Text, fileName.Text);
        return path;
    }
    CWideString GetProfileDir(const CWideString& name, bool create)
    {
        CWideString savedName = ProfileName;
        ProfileName = name;
        CWideString path = GetPathNameFromFileName(g_ProfileFileName, create);
        ProfileName = savedName;
        return path;
    }
};

int main()
{
    CUserProfileManager manager;
    CWideString other("Reaver");
    CWideString path = manager.GetProfileDir(other, true);
    if (std::strcmp(path.Text, "Profiles\\Reaver\\Profile.bin") != 0 || std::strcmp(manager.ProfileName.Text, "Hero") != 0 ||
        manager.LastCreate != 1) {
        std::printf("004082d0_TEST FAIL\n");
        return 1;
    }
    CWideString hero("Hero");
    CWideString again = manager.GetProfileDir(hero, false);
    if (std::strcmp(again.Text, "Profiles\\Hero\\Profile.bin") != 0 || manager.LastCreate != 0) {
        std::printf("004082d0_TEST FAIL\n");
        return 1;
    }
    std::printf("004082d0_TEST PASS\n");
    return 0;
}