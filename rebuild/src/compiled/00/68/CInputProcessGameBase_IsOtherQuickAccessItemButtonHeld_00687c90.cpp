// CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld  @ 0x00687c90
// bool __fastcall (CInputProcessGameBase* this, long param)

// NF153: MyPlayer is the shared engine/CPlayer.h CPlayer (FABLE_RETAIL_PLAYER); its GetJoystickDeviceNumber() const 0x00487510
//   (Ego_r / FableWin ?GetJoystickDeviceNumber@CPlayer@@QBEJXZ) replaces the stand-in Helper18::GetValue.
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"

// NF171 (2026-11-20, q11921): the callee 0x00446060 is Ego_r.pdb (0x00442FD0) + FableWin.pdb (0x01A9B210)
//   ?IsOtherQuickAccessItemButtonHeld@CGamePlayerInterface@@QBE_NJJ@Z (the landed self row's Leaf), declared by engine/CGamePlayerInterface.h
//   (retail path, NF171 opt-in FABLE_RETAIL_CGAMEPLAYERINTERFACE_ISOTHERQUICKACCESSITEMBUTTONHELD); PGamePlayerInterface is a CGamePlayerInterface* (engine/CInputProcessGameBase.h
//   +0x24). Was the stand-in ?Check@Helper24@@QAE_NJH@Z.
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_ISOTHERQUICKACCESSITEMBUTTONHELD
#include "engine/CGamePlayerInterface.h"

struct CInputProcessGameBase
{
    char pad0[0x18];
    CPlayer* MyPlayer;   // +0x18
    char pad1[0x24 - 0x1C];
    CGamePlayerInterface* PGamePlayerInterface;   // +0x24

    bool IsOtherQuickAccessItemButtonHeld(long param);
};

bool CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld(long param)
{
    return PGamePlayerInterface->IsOtherQuickAccessItemButtonHeld(param, MyPlayer->GetJoystickDeviceNumber());
}