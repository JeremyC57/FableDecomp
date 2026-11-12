#include <cstdio>
// Behaviour model: CPlayerManager::UnfreezePlayerControls(n) calls UnfreezeControls() on GetPlayer(n).
static int g_calls = 0;
static const void* g_player = 0;
class CPlayer { public: long Number; void UnfreezeControls() { ++g_calls; g_player = this; } };
class CPlayerManager {
public:
    CPlayer Players[3];
    CPlayer& GetPlayer(long number) { return Players[number]; }
    void UnfreezePlayerControls(long number);
};
void CPlayerManager::UnfreezePlayerControls(long number)
{
    GetPlayer(number).UnfreezeControls();
}
int main()
{
    CPlayerManager m;
    m.UnfreezePlayerControls(2);
    if (g_calls != 1 || g_player != &m.Players[2]) { std::printf("FAIL_00449a50\n"); return 1; }
    std::printf("PASS_00449a50\n");
    return 0;
}
