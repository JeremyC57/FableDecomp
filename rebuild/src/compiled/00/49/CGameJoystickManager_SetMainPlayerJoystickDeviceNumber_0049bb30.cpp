
#include "engine/CGameJoystickManager.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall SetMainPlayerJoystickDeviceNumber(CGameJoystickManager *manager, void * /*edx*/, int deviceNumber)
{
    manager->MainPlayerJoystickDeviceNumber = deviceNumber;
    manager->MainPlayerJoystickDeviceNumberSet = true;
}