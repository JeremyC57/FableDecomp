// Behaviour test for 0x00643FC8 NPlayerGui::CDrawAbilityButtons::GetSpecialMoveIndexForButton
#include <cstdio>
class CGamePlayerInterface { public:
    long Buttons[4];
    long GetAssignableSpecialMoveIndexForButton(long button)
    {
        for (long i = 0; i < 4; ++i)
            if (Buttons[i] == button) return i;
        return -1;
    }
};
class CPlayer { public: CGamePlayerInterface* PlayerInterface; };
class CPlayerManager { public:
    CPlayer* Players[2];
    long MainPlayer;
    CPlayer* GetMainPlayer() { return Players[MainPlayer]; }
};
class CAbilityButtonGuiDef { public: long ControllerButton; long MouseButton; };
struct CDrawAbilityButtons {
    CPlayerManager* PlayerManager;
    long GetSpecialMoveIndexForButton(const CAbilityButtonGuiDef& def)
    {
        CGamePlayerInterface* playerInterface = PlayerManager->GetMainPlayer()->PlayerInterface;
        return playerInterface->GetAssignableSpecialMoveIndexForButton(def.ControllerButton);
    }
};
int main()
{
    CGamePlayerInterface a = { { 7, 8, 9, 10 } };
    CGamePlayerInterface b = { { 10, 9, 8, 7 } };
    CPlayer p0 = { &a };
    CPlayer p1 = { &b };
    CPlayerManager manager;
    manager.Players[0] = &p0; manager.Players[1] = &p1; manager.MainPlayer = 1;
    CDrawAbilityButtons draw; draw.PlayerManager = &manager;
    CAbilityButtonGuiDef def = { 9, 3 };
    int ok = draw.GetSpecialMoveIndexForButton(def) == 1;
    manager.MainPlayer = 0;
    ok = ok && draw.GetSpecialMoveIndexForButton(def) == 2;
    def.ControllerButton = 99;
    ok = ok && draw.GetSpecialMoveIndexForButton(def) == -1;
    if (!ok) { std::printf("00643fc8_TEST FAIL\n"); return 1; }
    std::printf("00643fc8_TEST PASS\n");
    return 0;
}