// Behaviour test for 00633670 (CPlayer::UninitModeClickPastText): the mode's two input processes are removed from the interface, InputProcessYesNoQuestion_ptr first, then InputProcessRightStickLookAround_ptr.
#include <cstdio>
static const void* g_removed[4];
static int g_count = 0;
class CAInputProcess { public:
    int id;
    void RemoveFromInterface() { g_removed[g_count++] = this; }   // stands in for 0x00A0D2D0
};
typedef long EPlayerMode;
class CProcessedInput { public: int unused; };
class CPlayer { public:                            // the two input-process slots of the retail CPlayer
    CAInputProcess* InputProcessYesNoQuestion_ptr;
    CAInputProcess* InputProcessRightStickLookAround_ptr;
    void UninitModeClickPastText(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::UninitModeClickPastText(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessYesNoQuestion_ptr->RemoveFromInterface();
    InputProcessRightStickLookAround_ptr->RemoveFromInterface();
}
int main()
{
    CAInputProcess first, second;
    first.id = 1; second.id = 2;
    CPlayer player;
    player.InputProcessYesNoQuestion_ptr = &first;
    player.InputProcessRightStickLookAround_ptr = &second;
    CProcessedInput input;
    player.UninitModeClickPastText(3, &input);
    if (g_count != 2 || g_removed[0] != &first || g_removed[1] != &second) { std::printf("00633670_TEST FAIL\n"); return 1; }
    std::printf("00633670_TEST PASS\n");
    return 0;
}