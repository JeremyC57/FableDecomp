// CNewFrontendGameComponent::IsControllerDisconnected() -- true when an explicit main-player joystick is set and that device is no longer active
// Leaf: ?IsControllerDisconnected@CNewFrontendGameComponent@@AAE_NXZ
// Retail: Fable.exe @ 0x0042DC63, 49 bytes; Ego_r.pdb / FableWin.pdb ?IsControllerDisconnected@CNewFrontendGameComponent@@AAE_NXZ
// (private); caller 0x0042DCBF (CNewFrontendGameComponent::Update). Body: CGameJoystickManager::Get (0x0049BB90, static
// __fastcall) -> HasExplicitMainPlayerJoystickDeviceNumber (0x0049BB50, `mov al,[ecx+8]`); if set,
// !g_pInputManager (0x013B8388) -> CInputManager::IsJoystickDeviceNumberActive(GetMainPlayerJoystickDeviceNumber()
// 0x0049BB40) (0x009F5250); else false. `this` is unused.
// Layout: no members of `this` are read; CGameJoystickManager / CInputManager are modelled by their called methods only
// (as the landed users of 0x0049BB90 / 0x013B8388).
// Opt: size TU (unaligned entry after the 1-3 byte virtuals at 0x0042DC5C..0x0042DC60).
#pragma optimize("s", on)
class CGameJoystickManager {
public:
    static CGameJoystickManager& __fastcall Get();       // 0x0049BB90
    bool HasExplicitMainPlayerJoystickDeviceNumber() const;   // 0x0049BB50
    long GetMainPlayerJoystickDeviceNumber() const;      // 0x0049BB40
};
class CInputManager {
public:
    bool IsJoystickDeviceNumberActive(long device_number) const;   // 0x009F5250
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
class CNewFrontendGameComponent {
public:
    unsigned char _body[0x10];
private:
    bool IsControllerDisconnected();
    friend struct CNewFrontendGameComponent_Test;        // behaviour-test hook (the method is private)
};
bool CNewFrontendGameComponent::IsControllerDisconnected()
{
    if (CGameJoystickManager::Get().HasExplicitMainPlayerJoystickDeviceNumber())
        return !g_pInputManager->IsJoystickDeviceNumberActive(CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber());
    return false;
}