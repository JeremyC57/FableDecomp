// Behaviour test for CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities (0x00CBE09A): stores the interface
// and pauses the non-scripted entities through it (true).
#include <cstdio>

static int g_calls = 0; static bool g_last = false;

class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase() {}
    virtual void PauseAllNonScriptedEntities(bool pause) { ++g_calls; g_last = pause; }
};

class CWideScreenMagicPauseEntities {
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game);
    CGameScriptInterfaceBase* Game;
};

CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities(CGameScriptInterfaceBase* game)
    : Game(game)
{
    Game->PauseAllNonScriptedEntities(true);
}

int main()
{
    CGameScriptInterfaceBase game;
    CWideScreenMagicPauseEntities guard(&game);
    if (guard.Game != &game || g_calls != 1 || !g_last) { std::printf("00cbe09a_TEST FAIL\n"); return 1; }
    std::printf("00cbe09a_TEST PASS\n");
    return 0;
}