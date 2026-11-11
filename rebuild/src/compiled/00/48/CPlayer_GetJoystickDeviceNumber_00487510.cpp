// CPlayer::GetJoystickDeviceNumber() const -- the joystick device number the player is bound to
// Leaf: ?GetJoystickDeviceNumber@CPlayer@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487510, 7 bytes: `mov eax,[ecx+228h] ; ret`.
// Name (agentNF16): Ego_r.pdb / FableWin.pdb ?GetJoystickDeviceNumber@CPlayer@@QBEJXZ; Ego_r donor order IsFirstPersonTargeting
//   (0x00492CC0) -> IsUsingJoystickDeviceNumber -> GetJoystickDeviceNumber (0x00492D00) matches retail 0x004874D0 .. 0x00487510, and
//   the read is CPlayer::JoystickDeviceNumber (+0x228, engine/CPlayer.h). Was the free stand-in DrawGetEnvironment(const unsigned char*).
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"

long CPlayer::GetJoystickDeviceNumber() const
{
    return JoystickDeviceNumber;
}
