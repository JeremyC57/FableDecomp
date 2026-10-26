// CPlayer::InitModes() -- for a local player: the main player's main input process joins the player's interface, then the player enters the default mode (AddMode(0, true))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00636E50, length 60 bytes.
// Identity: catalog label CPlayer::InitModes.
// Callees: CPlayer::IsMainPlayer() const @ 0x00487520 (out of line in this module), the main
// process's virtual AddToInterface(CGamePlayerInterface*) (slot 1), CPlayer::AddMode(EPlayerMode,
// bool) @ 0x006345C0.
// Shape: engine/CPlayer.h members (PlayerInterface +0x0C, InputProcessMain_ptr +0x100, Local
// +0x208). Two `if (Local)` tests: the second re-reads the byte after the virtual call (a nested
// `if (IsMainPlayer())` inside the first, the AddMode guarded again exactly like the landed
// CPlayer::ResetModes 0x00634870).
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer

class CPlayer : public CPlayerLayout
{
public:
    bool IsMainPlayer() const;   // 0x00487520
};

typedef long EPlayerMode;

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CPlayer_Methods : CPlayer {
    void AddMode(EPlayerMode mode, bool exclusive);   // 0x006345C0
    void InitModes();
};

void CPlayer_Methods::InitModes()
{
    if (this->Local) {
        if (IsMainPlayer())
            reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessMain_ptr)->AddToInterface(this->PlayerInterface);
        if (this->Local)
            AddMode(0, true);
    }
}