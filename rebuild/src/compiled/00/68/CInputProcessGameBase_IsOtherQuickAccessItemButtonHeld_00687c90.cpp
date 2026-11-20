// CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld  @ 0x00687c90
// bool __fastcall (CInputProcessGameBase* this, long param)

// NF153: MyPlayer is the shared engine/CPlayer.h CPlayer (FABLE_RETAIL_PLAYER); its GetJoystickDeviceNumber() const 0x00487510
//   (Ego_r / FableWin ?GetJoystickDeviceNumber@CPlayer@@QBEJXZ) replaces the stand-in Helper18::GetValue.
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"

struct Helper24
{
    bool Check(long b, int a); // thiscall (b,a) -> 0x446060
};

struct CInputProcessGameBase
{
    char pad0[0x18];
    CPlayer* MyPlayer;   // +0x18
    char pad1[0x24 - 0x1C];
    Helper24* PGamePlayerInterface;   // +0x24

    bool IsOtherQuickAccessItemButtonHeld(long param);
};

bool CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld(long param)
{
    return PGamePlayerInterface->Check(param, MyPlayer->GetJoystickDeviceNumber());
}