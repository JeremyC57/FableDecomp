#include <cstdio>
#include <cstring>

// Model of 0x00406780 CUserProfileManager::GetDebugSaveDir: "Data\" prefixed to NResourceDirectoryNames::GetSaveDir()
// (operator+(const wchar*, const CWideString&) 0x0099BFF0); the string is modelled with narrow text.
class CWideString {
public:
    char Text[64];
    explicit CWideString(const char* text) { std::strcpy(Text, text); }
    CWideString(const CWideString& other) { std::strcpy(Text, other.Text); }
};
static CWideString operator+(const char* left, const CWideString& right)
{
    CWideString result(left);
    std::strcat(result.Text, right.Text);
    return result;
}
namespace NResourceDirectoryNames {
    static CWideString GetSaveDir() { return CWideString("Saves\\"); }
}
class CUserProfileManager {
public:
    CWideString GetDebugSaveDir() const { return "Data\\" + NResourceDirectoryNames::GetSaveDir(); }
};

int main()
{
    CUserProfileManager manager;
    CWideString dir = manager.GetDebugSaveDir();
    if (std::strcmp(dir.Text, "Data\\Saves\\") != 0) {
        std::printf("00406780_TEST FAIL\n");
        return 1;
    }
    std::printf("00406780_TEST PASS\n");
    return 0;
}