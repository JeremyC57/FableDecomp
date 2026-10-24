// Behaviour test for CPlayer::UninitModeParalysed (0x00633830): the six input processes are detached in init order.
#include <cstdio>
static int g_order[16]; static int g_count = 0;
struct CAInputProcess { int id; void RemoveFromInterface() { g_order[g_count++] = id; } };
struct CPlayer {
    CAInputProcess* InputProcessCreatureMovement_ptr;
    CAInputProcess* InputProcessDebugControls_ptr;
    CAInputProcess* InputProcessControlCreatureRightStick_ptr;
    CAInputProcess* InputProcessRightStickLookAround_ptr;
    CAInputProcess* InputProcessCycleSpecialCameraModes_ptr;
    CAInputProcess* InputProcessParalysed_ptr;
    void UninitModeParalysed(long mode, void* input)
    {
        (void)mode; (void)input;
        InputProcessParalysed_ptr->RemoveFromInterface();
        InputProcessControlCreatureRightStick_ptr->RemoveFromInterface();
        InputProcessRightStickLookAround_ptr->RemoveFromInterface();
        InputProcessCycleSpecialCameraModes_ptr->RemoveFromInterface();
        InputProcessCreatureMovement_ptr->RemoveFromInterface();
        InputProcessDebugControls_ptr->RemoveFromInterface();
    }
};
int main()
{
    CAInputProcess p[6]; for (int i = 0; i < 6; ++i) p[i].id = i + 1;
    CPlayer pl;
    pl.InputProcessParalysed_ptr = &p[0]; pl.InputProcessControlCreatureRightStick_ptr = &p[1];
    pl.InputProcessRightStickLookAround_ptr = &p[2]; pl.InputProcessCycleSpecialCameraModes_ptr = &p[3];
    pl.InputProcessCreatureMovement_ptr = &p[4]; pl.InputProcessDebugControls_ptr = &p[5];
    pl.UninitModeParalysed(7, 0);
    if (g_count != 6) { std::printf("00633830_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (g_order[i] != i + 1) { std::printf("00633830_TEST FAIL\n"); return 1; }
    std::printf("00633830_TEST PASS\n");
    return 0;
}