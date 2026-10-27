// behaviour test for 0x00E7B420 NScript::CV_FishermanScript::Init() (model: the listed members are cleared, the others untouched)
#include <cstdio>
#include <cstring>
struct S {
    bool IntroPlayed;
    bool HeroHasSavedFishermanFromWasps;
    int WaspsAlive;
    bool FishermanMustRunAway;
    bool QuestCardGiven;
    int FishermanTasks;
    void Init()
    {
        HeroHasSavedFishermanFromWasps = false;
        IntroPlayed = false;
        WaspsAlive = 0;
        QuestCardGiven = false;
        FishermanTasks = 0;
    }
};
int main()
{
    S s; std::memset(&s, 0x5A, sizeof(s));
    s.Init();
    int ok = 1;
    if (s.HeroHasSavedFishermanFromWasps != 0) ok = 0;
    if (s.IntroPlayed != 0) ok = 0;
    if (s.WaspsAlive != 0) ok = 0;
    if (s.QuestCardGiven != 0) ok = 0;
    if (s.FishermanTasks != 0) ok = 0;
    { bool v; std::memset(&v, 0x5A, sizeof(v)); if (std::memcmp(&v, &s.FishermanMustRunAway, sizeof(v)) != 0) ok = 0; }
    if (!ok) { std::printf("00e7b420_TEST FAIL\n"); return 1; }
    std::printf("00e7b420_TEST PASS\n");
    return 0;
}