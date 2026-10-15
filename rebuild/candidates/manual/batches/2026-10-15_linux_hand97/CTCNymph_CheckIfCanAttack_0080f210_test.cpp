// Behaviour test for CTCNymph::CheckIfCanAttack @ 0x0080F210: (frame - tick) * fps strictly above FlyAroundTime.
#include <cstdio>
static long g_GameFramesPerSecond = 15;
static long g_frame = 0;
struct CNymphDef { long FlyAroundTime; };
struct CTCNymph {
    const CNymphDef* PDef;
    bool CanAttack;
    void CheckIfCanAttack(unsigned long lastTargetTick)
    {
        long framesPerSecond = g_GameFramesPerSecond;
        if ((long)(g_frame - lastTargetTick) * framesPerSecond > PDef->FlyAroundTime) CanAttack = true;
    }
};
int main()
{
    CNymphDef d = { 150 };
    CTCNymph n; n.PDef = &d; n.CanAttack = false;
    g_frame = 110; n.CheckIfCanAttack(100);
    if (n.CanAttack) { std::printf("0080f210_TEST FAIL\n"); return 1; }
    g_frame = 111; n.CheckIfCanAttack(100);
    if (!n.CanAttack) { std::printf("0080f210_TEST FAIL\n"); return 1; }
    std::printf("0080f210_TEST PASS\n");
    return 0;
}
