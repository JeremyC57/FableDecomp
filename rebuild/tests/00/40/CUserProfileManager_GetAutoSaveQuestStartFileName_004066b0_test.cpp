// behaviour test for 0x004066B0: GetAutoSaveQuestStartFileName returns L"AutoSave.qs"
#include <cstdio>
#include <cstring>
class CWideString { public: char Text[32]; };
static CWideString Make(const char* a) { CWideString w; std::strcpy(w.Text, a); return w; }
static CWideString Add(const CWideString& l, const char* r) { CWideString w = l; std::strcat(w.Text, r); return w; }
class CUserProfileManager { public: CWideString GetAutoSaveQuestStartFileName() const { return Add(Make("AutoSave"), ".qs"); } };
int main()
{
    CUserProfileManager m;
    if (std::strcmp(m.GetAutoSaveQuestStartFileName().Text, "AutoSave.qs") != 0) { std::printf("004066b0_TEST FAIL\n"); return 1; }
    std::printf("004066b0_TEST PASS\n");
    return 0;
}