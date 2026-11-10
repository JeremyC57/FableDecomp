// Behaviour test for 0x005EDCF0 CTCInventoryAbilities::GetNumAssignableSpecialMoveButtons() const: the main player's
// game player interface answers with the size of its assignable special-move button list.
#include <stdio.h>
#include <vector>
class CGamePlayerInterface {
public:
    std::vector<long> AssignableSpecialMoveButtons;
    long GetNumberOfAssignableSpecialMoveButtons() const { return (long)AssignableSpecialMoveButtons.size(); }
};
class CPlayer {
public:
    CGamePlayerInterface* PlayerInterface;
};
class CPlayerManager {
public:
    CPlayer Players[4];
    long MainPlayer;
    const CPlayer& GetMainPlayer() const { return Players[MainPlayer]; }
};
static CPlayerManager g_playerManager;
class CTCInventoryAbilities {
public:
    const CPlayerManager& PeekPlayerManager() const { return g_playerManager; }
    long GetNumAssignableSpecialMoveButtons() const;
};
long CTCInventoryAbilities::GetNumAssignableSpecialMoveButtons() const
{
    return PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons();
}
int main()
{
    CGamePlayerInterface first, second;
    first.AssignableSpecialMoveButtons.resize(3);
    second.AssignableSpecialMoveButtons.resize(5);
    g_playerManager.Players[0].PlayerInterface = &first;
    g_playerManager.Players[1].PlayerInterface = &second;
    CTCInventoryAbilities abilities;
    g_playerManager.MainPlayer = 0;
    bool ok = abilities.GetNumAssignableSpecialMoveButtons() == 3;
    g_playerManager.MainPlayer = 1;
    ok = ok && abilities.GetNumAssignableSpecialMoveButtons() == 5;
    if (!ok) { printf("005edcf0_TEST FAIL\n"); return 1; }
    printf("005edcf0_TEST PASS\n");
    return 0;
}
