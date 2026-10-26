// Behaviour test for CThing::SetCutscenePauseBehaviour @ 0x004C78A0: the three behaviours set the two flags, others leave them.
#include <cstdio>
class CThing { public:
    unsigned char low : 7;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char high : 7;
    void SetCutscenePauseBehaviour(long behaviour)
    {
        switch (behaviour) {
        case 0: PausedDuringCutscenes = false; NotPausedDuringCutscenes = false; break;
        case 1: PausedDuringCutscenes = true;  NotPausedDuringCutscenes = false; break;
        case 2: PausedDuringCutscenes = false; NotPausedDuringCutscenes = true;  break;
        }
    }
};
int main()
{
    CThing t; t.low = 0x55; t.high = 0x2a; t.PausedDuringCutscenes = 1; t.NotPausedDuringCutscenes = 1;
    t.SetCutscenePauseBehaviour(7);
    if (!t.PausedDuringCutscenes || !t.NotPausedDuringCutscenes) { std::printf("004c78a0_TEST FAIL\n"); return 1; }
    t.SetCutscenePauseBehaviour(0);
    if (t.PausedDuringCutscenes || t.NotPausedDuringCutscenes) { std::printf("004c78a0_TEST FAIL\n"); return 1; }
    t.SetCutscenePauseBehaviour(1);
    if (!t.PausedDuringCutscenes || t.NotPausedDuringCutscenes) { std::printf("004c78a0_TEST FAIL\n"); return 1; }
    t.SetCutscenePauseBehaviour(2);
    if (t.PausedDuringCutscenes || !t.NotPausedDuringCutscenes || t.low != 0x55 || t.high != 0x2a) { std::printf("004c78a0_TEST FAIL\n"); return 1; }
    std::printf("004c78a0_TEST PASS\n");
    return 0;
}