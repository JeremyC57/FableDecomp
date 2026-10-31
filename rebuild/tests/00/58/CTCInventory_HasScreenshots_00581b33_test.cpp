// behaviour test for CTCInventory::HasScreenshots 0x00581B33: builds <save dir><valid profile name>\*.jpg and returns the
// first-file scan result for that pattern
#include <cstdio>
#include <string>
static std::wstring g_seenPattern;
static bool g_scanResult = false;
class CWideString { public: std::wstring s; };
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r; r.s = a.s + b.s; return r; }
static CWideString operator+(const CWideString& a, const wchar_t* b) { CWideString r; r.s = a.s + b; return r; }
static CWideString GetValidProfileName(const CWideString& n) { CWideString r; r.s = n.s + L"_ok"; return r; }
static CWideString GetSaveDir() { CWideString r; r.s = L"C:\\Saves\\"; return r; }
class CUserProfileManager { public: int pad[2]; CWideString ProfileName; };
static CUserProfileManager g_mgr;
static CUserProfileManager& Get() { return g_mgr; }
struct CFileScanInfo { bool Stopped; long SearchHandle; };
static bool StartFileScan(const CWideString& pattern, CFileScanInfo&, wchar_t*) { g_seenPattern = pattern.s; return g_scanResult; }
static bool HasScreenshots()
{
    CUserProfileManager& profiles = Get();
    CWideString pattern = GetSaveDir() + GetValidProfileName(profiles.ProfileName) + L"\\" + L"*.jpg";
    CFileScanInfo scanInfo;
    wchar_t fileName[0x20C / 2];
    bool found = StartFileScan(pattern, scanInfo, fileName);
    return found;
}
int main()
{
    g_mgr.ProfileName.s = L"Hero";
    g_scanResult = false;
    if (HasScreenshots() || g_seenPattern != L"C:\\Saves\\Hero_ok\\*.jpg") { std::printf("00581b33_TEST FAIL\n"); return 1; }
    g_scanResult = true;
    if (!HasScreenshots()) { std::printf("00581b33_TEST FAIL\n"); return 1; }
    std::printf("00581b33_TEST PASS\n");
    return 0;
}