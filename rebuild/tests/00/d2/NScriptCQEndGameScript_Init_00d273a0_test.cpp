#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct Script {
    float UndeadBar; int GuiCounter; bool SeenAbbey, QuestStartScreened, SeenMotherDisappear, MotherSpoken, ShowUndeadBar, After;
    void Init()
    {
        UndeadBar = 0.0f;
        GuiCounter = 0;
        SeenAbbey = false;
        QuestStartScreened = false;
        SeenMotherDisappear = false;
        MotherSpoken = false;
        ShowUndeadBar = false;
    }
};
int main()
{
    int ok = 1;
    Script s; s.UndeadBar = 3.5f; s.GuiCounter = 4; s.SeenAbbey = s.QuestStartScreened = s.SeenMotherDisappear = s.MotherSpoken = s.ShowUndeadBar = s.After = true;
    s.Init();
    ok = s.UndeadBar == 0.0f && s.GuiCounter == 0 && !s.SeenAbbey && !s.QuestStartScreened && !s.SeenMotherDisappear && !s.MotherSpoken && !s.ShowUndeadBar && s.After;
    if (!ok) { std::printf("00d273a0_TEST FAIL\n"); return 1; }
    std::printf("00d273a0_TEST PASS\n");
    return 0;
}