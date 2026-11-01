// CPlayer::IsUsingJoystickDeviceNumber  retail 0x004874f0
// __fastcall bool(CPlayer*, long)

class CPlayerManager { public:
    long GetPlayerNumberFromJoystickDeviceNumber(long a0) const;   // 0x00449990 (Ego_r.pdb ?GetPlayerNumberFromJoystickDeviceNumber@CPlayerManager@@QBEJJ@Z)
};
struct CInputThing {
};

class CPlayer { public:
	char pad0[0x10];
	CInputThing *PlayerManager;   // +0x10
	char pad14[0x28 - 0x14];
	long Number;           // +0x28  stored joystick device number
	bool IsUsingJoystickDeviceNumber(long which);
};

bool CPlayer::IsUsingJoystickDeviceNumber(long which)
{
	long stored = this->Number;
	if (((CPlayerManager*)(this->PlayerManager))->GetPlayerNumberFromJoystickDeviceNumber(which) == stored)
		return true;
	return false;
}