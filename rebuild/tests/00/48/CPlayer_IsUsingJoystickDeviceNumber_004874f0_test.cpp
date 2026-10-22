#include <cstdio>

struct CInputThing {
	long val;
	long GetDeviceNumber(long which) { return val + which; }
};

struct CPlayer {
	char pad0[0x10];
	CInputThing *PlayerManager;
	char pad14[0x28 - 0x14];
	long Number;
	bool IsUsingJoystickDeviceNumber(long which);
};

bool CPlayer::IsUsingJoystickDeviceNumber(long which)
{
	long stored = this->Number;
	if (this->PlayerManager->GetDeviceNumber(which) == stored)
		return true;
	return false;
}

int main()
{
	CInputThing device;
	device.val = 5;
	CPlayer p;
	CInputThing *pd = &device;
	p.PlayerManager = pd;
	p.Number = 7;   // 5+2==7 -> true for which=2
	bool a = p.IsUsingJoystickDeviceNumber(2);
	bool b = p.IsUsingJoystickDeviceNumber(3);  // 5+3=8 != 7 -> false
	if (a && !b) {
		printf("JOYDEV_PASS\n");
		return 0;
	}
	printf("JOYDEV_FAIL\n");
	return 1;
}