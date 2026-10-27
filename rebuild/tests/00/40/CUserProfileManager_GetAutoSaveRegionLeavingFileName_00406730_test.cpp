// behaviour test for 0x00406730: GetAutoSaveRegionLeavingFileName returns L"AutoSave.rl"
#include <cstdio>
#include <cstring>
class CWideString { public: char Text[32]; };
static CWideString Make(const char* a) { CWideString w; std::strcpy(w.Text, a); return w; }
static CWideString Add(const CWideString& l, const char* r) { CWideString w = l; std::strcat(w.Text, r); return w; }
class CUserProfileManager { public: CWideString GetAutoSaveRegionLeavingFileName() const { return Add(Make("AutoSave"), ".rl"); } };
int main()
{
    CUserProfileManager m;
    if (std::strcmp(m.GetAutoSaveRegionLeavingFileName().Text, "AutoSave.rl") != 0) { std::printf("00406730_TEST FAIL\n"); return 1; }
    std::printf("00406730_TEST PASS\n");
    return 0;
}