#include <cstdio>
enum EPlayerMode { PLAYER_MODE_3 = 3 };
struct CPlayerManager { long m_main; long GetMainPlayerNumber() const { return m_main; } };
struct CThing { int m_killed; bool m_silently; void Kill(bool silently) { ++m_killed; m_silently = silently; } };
struct CIntelligentPointer_CThing { CThing* m_pPointer; CThing* GetPItem() const { return m_pPointer; } };
struct CPlayer {
    CPlayerManager* PlayerManager; long Number; CIntelligentPointer_CThing PlayerCharacter;
    int m_resets; int m_lastMode; bool m_lastFlag;
    bool IsMainPlayer() const { long number = Number; return number == PlayerManager->GetMainPlayerNumber(); }
    void ResetModes() { ++m_resets; }
    void AddMode(EPlayerMode mode, bool flag) { m_lastMode = mode; m_lastFlag = flag; }
    void UninitCharacter()
    {
        if (!IsMainPlayer()) ResetModes(); else AddMode(PLAYER_MODE_3, true);
        if (PlayerCharacter.GetPItem()) PlayerCharacter.GetPItem()->Kill(false);
    }
};
int main()
{
    CPlayerManager pm; pm.m_main = 0;
    CThing hero; hero.m_killed = 0; hero.m_silently = true;
    CPlayer p; p.PlayerManager = &pm; p.Number = 0; p.PlayerCharacter.m_pPointer = &hero; p.m_resets = 0; p.m_lastMode = -1; p.m_lastFlag = false;
    p.UninitCharacter();
    if (p.m_resets != 0 || p.m_lastMode != 3 || !p.m_lastFlag || hero.m_killed != 1 || hero.m_silently) { std::printf("00487bd0_TEST FAIL\n"); return 1; }
    p.Number = 1; p.PlayerCharacter.m_pPointer = 0; p.m_lastMode = -1;
    p.UninitCharacter();
    if (p.m_resets != 1 || p.m_lastMode != -1 || hero.m_killed != 1) { std::printf("00487bd0_TEST FAIL\n"); return 1; }
    std::printf("00487bd0_TEST PASS\n");
    return 0;
}