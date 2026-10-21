// CMainGameComponent::IsControllerDisconnected() const -- true when the main player's joystick device is not active
// Leaf: IsControllerDisconnected@CMainGameComponent
// Retail: Fable.exe @ 0x00416296, 31 bytes; Ego_r.pdb ?IsControllerDisconnected@CMainGameComponent@@QBE_NXZ. Body:
// !g_pInputManager (0x013B8388)->IsJoystickDeviceNumberActive(CGameJoystickManager::Get() (0x0049BB90, static fastcall,
// vptr 0x01238B54).GetMainPlayerJoystickDeviceNumber() (0x0049BB40, `mov eax,[ecx+4]`)) (0x009F5250); the argument is
// evaluated before the object load. Only caller 0x004182D9.
// Layout: the method touches no CMainGameComponent member (no engine header exists for the class).
// Shape: `cond ? false : true` gives the byte `sbb al,al ; inc al`; `!cond` / `== false` give `sbb eax,eax ; inc eax`.
class CGameJoystickManager {
public:
    static CGameJoystickManager& __fastcall Get();       // 0x0049BB90
    long GetMainPlayerJoystickDeviceNumber() const;      // 0x0049BB40
};
class CInputManager {
public:
    bool IsJoystickDeviceNumberActive(long device) const;   // 0x009F5250
};
extern CInputManager* g_pInputManager;                  // 0x013B8388
class CMainGameComponent {
public:
    bool IsControllerDisconnected() const;
};
bool CMainGameComponent::IsControllerDisconnected() const
{
    return g_pInputManager->IsJoystickDeviceNumberActive(CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()) ? false : true;
}