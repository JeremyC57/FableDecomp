// Behaviour test for 0x0041A1A0 NResourceDirectoryNames::GetDebugSaveDir() (forwards to the profile manager singleton).
#include <cstdio>
#include <cstring>

class CWideString {
public:
    const wchar_t* PData;
    CWideString(const wchar_t* s) : PData(s) {}
};
class CUserProfileManager {
public:
    const wchar_t* SaveDir;
    static int s_gets;
    static CUserProfileManager& Get() { static CUserProfileManager m; ++s_gets; return m; }
    CWideString GetDebugSaveDir() const { return CWideString(SaveDir); }
};
int CUserProfileManager::s_gets = 0;
namespace NResourceDirectoryNames {
CWideString GetDebugSaveDir() { return CUserProfileManager::Get().GetDebugSaveDir(); }
}

int main()
{
    static const wchar_t dir[] = L"Saves\\Data\\";
    CUserProfileManager::Get().SaveDir = dir;
    CWideString d = NResourceDirectoryNames::GetDebugSaveDir();
    if (!(CUserProfileManager::s_gets == 2 && d.PData == dir)) {
        std::printf("0041a1a0_TEST FAIL\n");
        return 1;
    }
    std::printf("0041a1a0_TEST PASS\n");
    return 0;
}