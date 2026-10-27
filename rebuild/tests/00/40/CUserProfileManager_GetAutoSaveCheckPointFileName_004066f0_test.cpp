// behaviour test for 0x004066F0: GetAutoSaveCheckPointFileName returns L"AutoSave.cp"
#include <cstdio>
#include <cstring>
class CWideString { public: char Text[32]; };
static CWideString Make(const char* a) { CWideString w; std::strcpy(w.Text, a); return w; }
static CWideString Add(const CWideString& l, const char* r) { CWideString w = l; std::strcat(w.Text, r); return w; }
class CUserProfileManager { public: CWideString GetAutoSaveCheckPointFileName() const { return Add(Make("AutoSave"), ".cp"); } };
int main()
{
    CUserProfileManager m;
    if (std::strcmp(m.GetAutoSaveCheckPointFileName().Text, "AutoSave.cp") != 0) { std::printf("004066f0_TEST FAIL\n"); return 1; }
    std::printf("004066f0_TEST PASS\n");
    return 0;
}