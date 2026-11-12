// CInputProcessGameBase::IsQuickAccessEventInQueue(long button, EInputButtonEventType type) const -- forward to the player interface (+0x24) with this player's joystick device number (MyPlayer +0x18) as the third argument
// Leaf: ?IsQuickAccessEventInQueue@CInputProcessGameBase@@IBE_NJW4EInputButtonEventType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687CB0, length 34 bytes (+ int3 padding).
// Name (FAM13, queue land1011dt 0x004460C0): Ego_r.pdb / FableWin.pdb ?IsQuickAccessEventInQueue@CInputProcessGameBase@@IBE_NJW4EInputButtonEventType@@@Z
//   (Ego_r 0x006B2DC0); neither PDB has a CInputProcessGameBase::IsAssignableSpecialMoveButtonInQueue (the old spelling). The callee
//   0x004460C0 scans QuickAccessItemButtons +0x7c4, so it is CGamePlayerInterface::IsQuickAccessEventInQueue (its twin over
//   AssignableSpecialMoveButtons +0x7b8 is 0x00445E90); the neighbour 0x00687C90 IsOtherQuickAccessItemButtonHeld is the same family.
// Callees: CPlayer::GetJoystickDeviceNumber @ 0x00487510 (?GetJoystickDeviceNumber@CPlayer@@QBEJXZ), CGamePlayerInterface::IsQuickAccessEventInQueue @ 0x004460C0.
// The catalog label "CPersistContext::Transfer<CEngineGraphic>" is propagated: the body is a CInputProcessGameBase
// forwarder like its landed neighbours 0x00687C90 / 0x00687D80.
// Shape: the arguments are evaluated right to left -- the device-number call first (ecx = MyPlayer), then the two
// words reloaded from their slots; the bool rides back through al with no tail jump (esi restored); `ret 8`.
// Layout: engine/CInputProcessGameBase.h (PDB): MyPlayer +0x18, PGamePlayerInterface +0x24; the stand-ins CPlayerCalls /
//   CGamePlayerInterfaceCalls / CInputProcessGameBase_Methods replaced by the shared headers (FAM13).
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSQUEUE
#include "engine/CGamePlayerInterface.h"
#define FABLE_RETAIL_INPUTPROCESS_QUICKACCESSQUEUE
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::IsQuickAccessEventInQueue(long button, EInputButtonEventType type) const
{
    return PGamePlayerInterface->IsQuickAccessEventInQueue(button, type, MyPlayer->GetJoystickDeviceNumber());
}
