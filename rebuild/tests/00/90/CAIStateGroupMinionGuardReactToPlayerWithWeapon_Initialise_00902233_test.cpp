#include <cstdio>
// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Initialise: zero three longs, null the player pointer, base init
struct Ptr { void* item; void SetPItem(void* p) { item = p; } void* GetPItem() const { return item; } };
struct Base { int brainId; int priority; void Initialise(int brain, int prio) { brainId = brain; priority = prio; } };
struct Group : Base {
    void* warningSource; long frameWarned; long lastWarningId; Ptr player;
    void Initialise(int brain, int prio)
    {
        warningSource = 0; frameWarned = 0; lastWarningId = 0;
        Ptr temp; temp.SetPItem(0); player.SetPItem(temp.GetPItem());
        Base::Initialise(brain, prio);
    }
};
int main()
{
    int x; Group g; g.warningSource = &x; g.frameWarned = 7; g.lastWarningId = 9; g.player.item = &x; g.brainId = 0; g.priority = 0;
    g.Initialise(3, 4);
    if (g.warningSource != 0 || g.frameWarned != 0 || g.lastWarningId != 0 || g.player.item != 0 || g.brainId != 3 || g.priority != 4) { std::printf("00902233_TEST FAIL\n"); return 1; }
    std::printf("00902233_TEST PASS\n"); return 0;
}