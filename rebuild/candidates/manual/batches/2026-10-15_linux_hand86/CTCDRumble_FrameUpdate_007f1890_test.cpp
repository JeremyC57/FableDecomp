// Behaviour test for CTCDRumble::FrameUpdate() @ 0x007F1890: the timer counts down every frame and the parent is
// killed (Kill(true)) on the frame the timer was already negative.
#include <cstdio>
static int g_kills = 0;
static bool g_killArg = false;
struct CThing { void Kill(bool immediately) { ++g_kills; g_killArg = immediately; } };
struct CTCDRumble {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    void* PDef; float Strength; float Length; bool GlobalRumble; long LifeTimer;
    void FrameUpdate();
};
void CTCDRumble::FrameUpdate()
{
    if (LifeTimer-- < 0)
        ParentThing->Kill(true);
}
int main()
{
    CThing thing; CTCDRumble r; r.ParentThing = &thing; r.LifeTimer = 1;
    r.FrameUpdate();                      // 1 -> 0, no kill
    if (g_kills != 0 || r.LifeTimer != 0) { std::printf("007f1890_TEST FAIL\n"); return 1; }
    r.FrameUpdate();                      // 0 -> -1, no kill
    if (g_kills != 0 || r.LifeTimer != -1) { std::printf("007f1890_TEST FAIL\n"); return 1; }
    r.FrameUpdate();                      // -1 -> -2, kill
    if (g_kills != 1 || g_killArg != true || r.LifeTimer != -2) { std::printf("007f1890_TEST FAIL\n"); return 1; }
    std::printf("007f1890_TEST PASS\n");
    return 0;
}
