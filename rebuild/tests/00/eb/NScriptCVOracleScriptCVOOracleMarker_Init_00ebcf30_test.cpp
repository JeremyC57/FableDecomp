// behaviour test for 0x00EBCF30 NScript::CV_OracleScript::CVO_OracleMarker::Init() (model: the listed members are cleared, the others untouched)
#include <cstdio>
#include <cstring>
struct S {
    int TimesPlayedYCat;
    int TimesPlayedMCat;
    int TimesPlayedCCat;
    int TimesPlayedACat;
    bool HaveBeenToldHowToInteract;
    void Init()
    {
        TimesPlayedYCat = 0;
        TimesPlayedMCat = 0;
        TimesPlayedCCat = 0;
        TimesPlayedACat = 0;
        HaveBeenToldHowToInteract = false;
    }
};
int main()
{
    S s; std::memset(&s, 0x5A, sizeof(s));
    s.Init();
    int ok = 1;
    if (s.TimesPlayedYCat != 0) ok = 0;
    if (s.TimesPlayedMCat != 0) ok = 0;
    if (s.TimesPlayedCCat != 0) ok = 0;
    if (s.TimesPlayedACat != 0) ok = 0;
    if (s.HaveBeenToldHowToInteract != 0) ok = 0;
    if (!ok) { std::printf("00ebcf30_TEST FAIL\n"); return 1; }
    std::printf("00ebcf30_TEST PASS\n");
    return 0;
}