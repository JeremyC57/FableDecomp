#include <cstdio>
// Behaviour model: CPlayerManager::FreezePlayerControls(n) calls FreezeControls() on GetPlayer(n).
static int g_calls = 0;
static const void* g_player = 0;
class CPlayer { public: long Number; void FreezeControls() { ++g_calls; g_player = this; } };
class CPlayerManager {
public:
    CPlayer Players[3];
    CPlayer& GetPlayer(long number) { return Players[number]; }
    void FreezePlayerControls(long number);
};
void CPlayerManager::FreezePlayerControls(long number)
{
    GetPlayer(number).FreezeControls();
}
int main()
{
    CPlayerManager m;
    m.FreezePlayerControls(2);
    if (g_calls != 1 || g_player != &m.Players[2]) { std::printf("FAIL_00449a30\n"); return 1; }
    std::printf("PASS_00449a30\n");
    return 0;
}
