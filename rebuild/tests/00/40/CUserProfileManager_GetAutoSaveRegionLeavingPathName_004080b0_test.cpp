// behaviour test for 0x004080B0: GetAutoSaveRegionLeavingPathName = directory + L"AutoSave.rl"
#include <cstdio>
#include <cstring>
class CWideString { public: char Text[64]; };
static CWideString Make(const char* a) { CWideString w; std::strcpy(w.Text, a); return w; }
static CWideString Add(const CWideString& l, const char* r) { CWideString w = l; std::strcat(w.Text, r); return w; }
static CWideString Add2(const CWideString& l, const CWideString& r) { return Add(l, r.Text); }
class CUserProfileManager { public:
    CWideString GetDirectoryNameFromProfileName() const { return Make("Saves\\Hero\\"); }
    CWideString GetAutoSaveRegionLeavingFileName() const { CWideString name = Make("AutoSave"); return Add(name, ".rl"); }
    CWideString GetAutoSaveRegionLeavingPathName() const { CWideString d = GetDirectoryNameFromProfileName(); return Add2(d, GetAutoSaveRegionLeavingFileName()); }
};
int main()
{
    CUserProfileManager m;
    if (std::strcmp(m.GetAutoSaveRegionLeavingPathName().Text, "Saves\\Hero\\AutoSave.rl") != 0) { std::printf("004080b0_TEST FAIL\n"); return 1; }
    std::printf("004080b0_TEST PASS\n");
    return 0;
}