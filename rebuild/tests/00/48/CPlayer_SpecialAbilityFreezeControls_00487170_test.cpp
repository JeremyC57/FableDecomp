#include <cstdio>
// Behaviour model: CPlayer::SpecialAbilityFreezeControls adds mode 18 (PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS) with the flag false.
enum EPlayerMode { PLAYER_MODE_FREEZE_CONTROLS = 17, PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS = 18 };
static int g_mode = -1, g_flag = -1, g_calls = 0;
static const void* g_this = 0;
class CPlayer {
public:
    int Pad;
    void AddMode(EPlayerMode mode, bool flag) { g_this = this; g_mode = mode; g_flag = flag; ++g_calls; }
    void SpecialAbilityFreezeControls();
};
void CPlayer::SpecialAbilityFreezeControls()
{
    AddMode(PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS, false);
}
int main()
{
    CPlayer p;
    p.SpecialAbilityFreezeControls();
    if (g_calls != 1 || g_this != &p || g_mode != 18 || g_flag != 0) { std::printf("00487170_TEST FAIL\n"); return 1; }
    std::printf("00487170_TEST PASS\n");
    return 0;
}
