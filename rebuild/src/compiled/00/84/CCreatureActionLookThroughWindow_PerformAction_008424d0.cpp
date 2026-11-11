// CCreatureAction_LookThroughWindow::PerformAction() -- puts the action's player into mode 16 (not exclusive)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008424D0, length 17 bytes.
// Callees: CCreatureActionBase::GetPlayer @ 0x00692930 (thiscall, no arguments; the manifest label of that address is a propagated STL name), CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0 (landed naming, 0x00634870).
// Shape: the two arguments are pushed before the object expression is evaluated, then a plain call/ret.
#include "engine/EPlayerMode.h"
class CPlayer { public:
    void AddMode(EPlayerMode mode, bool exclusive);   // 0x006345C0 Ego_r/FableWin ?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    CPlayer* GetPlayer();                   // 0x00692930
};
class CCreatureAction_LookThroughWindow : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_LookThroughWindow::PerformAction()
{
    GetPlayer()->AddMode(PLAYER_MODE_FIRST_PERSON, false);
}