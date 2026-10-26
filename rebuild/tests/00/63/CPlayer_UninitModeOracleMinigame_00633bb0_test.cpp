// Behaviour test for 00633bb0 (CPlayer::UninitModeOracleMinigame): the mode's two input processes are removed from the interface, InputProcessOracleMinigame_ptr first, then InputProcessQuickAccessItems_ptr.
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
    CAInputProcess* InputProcessOracleMinigame_ptr;
    CAInputProcess* InputProcessQuickAccessItems_ptr;
    void UninitModeOracleMinigame(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::UninitModeOracleMinigame(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessOracleMinigame_ptr->RemoveFromInterface();
    InputProcessQuickAccessItems_ptr->RemoveFromInterface();
}
int main()
{
    CAInputProcess first, second;
    first.id = 1; second.id = 2;
    CPlayer player;
    player.InputProcessOracleMinigame_ptr = &first;
    player.InputProcessQuickAccessItems_ptr = &second;
    CProcessedInput input;
    player.UninitModeOracleMinigame(3, &input);
    if (g_count != 2 || g_removed[0] != &first || g_removed[1] != &second) { std::printf("00633bb0_TEST FAIL\n"); return 1; }
    std::printf("00633bb0_TEST PASS\n");
    return 0;
}