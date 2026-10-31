// Behaviour test for NScript::CV_MurderTwistScript::GemsFoundProcess (0x00EAD9F0): same control flow as src/00ead9f0.cpp
// over a mock script interface (inventory count per frame, thread-termination flag, rumble list of thing ids).
#include <cstdio>
struct MockGame {
    int frame; int diamondFrame; int gemId;
    long GetNumberOfItemsOfTypeInInventory() const { return frame >= diamondFrame ? 1 : 0; }
    void NewScriptFrame() { ++frame; }
    int GetThingWithScriptName() const { return gemId; }
};
struct Script {
    MockGame* Game; int terminateAtFrame; long MurderTwistItemsFound;
    int list[8]; int count;
    bool IsActiveThreadTerminating() const { return Game->frame >= terminateAtFrame; }
    void erase(int i) { for (int j = i; j + 1 < count; ++j) list[j] = list[j + 1]; --count; }
    void GemsFoundProcess()
    {
        while (Game->GetNumberOfItemsOfTypeInInventory() <= 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ++MurderTwistItemsFound;
        for (int i = 0; i != count; ++i) {
            if (Game->GetThingWithScriptName() == list[i]) {
                erase(i);
                return;
            }
        }
    }
};
int main()
{
    MockGame g = { 0, 3, 42 };
    Script s; s.Game = &g; s.terminateAtFrame = 100; s.MurderTwistItemsFound = 0;
    s.list[0] = 7; s.list[1] = 42; s.list[2] = 9; s.count = 3;
    s.GemsFoundProcess();
    if (g.frame != 3 || s.MurderTwistItemsFound != 1 || s.count != 2 || s.list[0] != 7 || s.list[1] != 9) {
        std::printf("00ead9f0_TEST FAIL\n"); return 1;
    }
    MockGame g2 = { 0, 50, 42 };
    Script t; t.Game = &g2; t.terminateAtFrame = 5; t.MurderTwistItemsFound = 0; t.count = 1; t.list[0] = 42;
    t.GemsFoundProcess();
    if (t.MurderTwistItemsFound != 0 || t.count != 1 || g2.frame != 5) { std::printf("00ead9f0_TEST FAIL\n"); return 1; }
    MockGame g3 = { 0, 0, 5 };
    Script u; u.Game = &g3; u.terminateAtFrame = 100; u.MurderTwistItemsFound = 2; u.count = 2; u.list[0] = 1; u.list[1] = 2;
    u.GemsFoundProcess();
    if (u.MurderTwistItemsFound != 3 || u.count != 2) { std::printf("00ead9f0_TEST FAIL\n"); return 1; }
    std::printf("00ead9f0_TEST PASS\n");
    return 0;
}