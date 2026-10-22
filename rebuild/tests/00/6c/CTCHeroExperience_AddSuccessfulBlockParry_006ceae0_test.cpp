#include <cstdio>
struct CTCHeroExperience { char pad[0xc]; int FishingLevel; };
static int g_second = 0x1234;
void First_Helper() { }
int Second_Helper() { return g_second; }
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    First_Helper();
    self->FishingLevel = Second_Helper();
}
int main()
{
    CTCHeroExperience obj;
    obj.FishingLevel = 0;
    CTCHeroExperience_AddSuccessfulBlockParry(&obj);
    if (obj.FishingLevel == 0x1234) { std::printf("CTCHeroExperience_006ceae0_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}