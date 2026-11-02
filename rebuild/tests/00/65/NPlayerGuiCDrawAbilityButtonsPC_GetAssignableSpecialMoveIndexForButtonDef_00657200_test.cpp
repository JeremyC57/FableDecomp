// Behaviour test for NPlayerGui::CDrawAbilityButtonsPC::GetAssignableSpecialMoveIndexForButtonDef (0x00657200): the PC override
// asks the main player's interface for the slot assigned to the def's MOUSE button (the base override uses the controller button).
#include <cstdio>
enum EMouseButtonControl { MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE };
class CGamePlayerInterface {
public:
    long Slots[3];
    long GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl button) const { return Slots[button]; }
};
class CPlayer { public: CGamePlayerInterface* PlayerInterface; };
class CPlayerManager { public: CPlayer* Main; CPlayer& GetMainPlayer() { return *Main; } };
class CAbilityButtonGuiDef { public: long ControllerButton; EMouseButtonControl MouseButton; };
class CDrawBase {
public:
    virtual ~CDrawBase() {}
    const CPlayer& GetPlayer() const { return PlayerManager->GetMainPlayer(); }
    CPlayerManager* PlayerManager;
};
class CDrawAbilityButtonsPC : public CDrawBase {
public:
    virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& def) const
    {
        CGamePlayerInterface* playerInterface = GetPlayer().PlayerInterface;
        return playerInterface->GetAssignableSpecialMoveIndexForMouseButton(def.MouseButton);
    }
};
int main()
{
    CGamePlayerInterface gpi; gpi.Slots[0] = 4; gpi.Slots[1] = 7; gpi.Slots[2] = -1;
    CPlayer player; player.PlayerInterface = &gpi;
    CPlayerManager pm; pm.Main = &player;
    CDrawAbilityButtonsPC draw; draw.PlayerManager = &pm;
    CAbilityButtonGuiDef def; def.ControllerButton = 2; def.MouseButton = MOUSE_RIGHT;
    CAbilityButtonGuiDef def2; def2.ControllerButton = 1; def2.MouseButton = MOUSE_MIDDLE;
    if (draw.GetAssignableSpecialMoveIndexForButtonDef(def) != 7 || draw.GetAssignableSpecialMoveIndexForButtonDef(def2) != -1) {
        std::printf("00657200_TEST FAIL\n"); return 1;
    }
    std::printf("00657200_TEST PASS\n");
    return 0;
}