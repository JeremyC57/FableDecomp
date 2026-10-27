// behaviour test for 0x00408040: GetAutoSaveCheckPointPathName = directory + L"AutoSave.cp"
#include <cstdio>
#include <cstring>
class CWideString { public: char Text[64]; };
static CWideString Make(const char* a) { CWideString w; std::strcpy(w.Text, a); return w; }
static CWideString Add(const CWideString& l, const char* r) { CWideString w = l; std::strcat(w.Text, r); return w; }
static CWideString Add2(const CWideString& l, const CWideString& r) { return Add(l, r.Text); }
class CUserProfileManager { public:
    CWideString GetDirectoryNameFromProfileName() const { return Make("Saves\\Hero\\"); }
    CWideString GetAutoSaveCheckPointFileName() const { CWideString name = Make("AutoSave"); return Add(name, ".cp"); }
    CWideString GetAutoSaveCheckPointPathName() const { CWideString d = GetDirectoryNameFromProfileName(); return Add2(d, GetAutoSaveCheckPointFileName()); }
};
int main()
{
    CUserProfileManager m;
    if (std::strcmp(m.GetAutoSaveCheckPointPathName().Text, "Saves\\Hero\\AutoSave.cp") != 0) { std::printf("00408040_TEST FAIL\n"); return 1; }
    std::printf("00408040_TEST PASS\n");
    return 0;
}