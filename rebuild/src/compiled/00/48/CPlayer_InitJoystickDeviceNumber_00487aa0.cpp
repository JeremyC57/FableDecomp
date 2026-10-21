// CPlayer::InitJoystickDeviceNumber() -- the main player takes the joystick manager's main-player device; any other player keeps its own number as device unless that collides with the main device (then 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487AA0, length 67 bytes.
// Identity: Ego_r.pdb `?InitJoystickDeviceNumber@CPlayer@@QAEXXZ`.
// Callees: CGameJoystickManager::Get() @ 0x0049BB90 (static `__fastcall`, returns a reference),
// CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const @ 0x0049BB40 (`mov eax,[ecx+4]`),
// CPlayerManager::GetMainPlayerNumber() const @ 0x00449700 (`mov eax,[ecx+1Ch]`; both manifest
// labels "CWorld::DrawGetEnvironment" are propagated).
// CPlayer::IsMainPlayer() const (out of line @ 0x00487520: Number read into esi BEFORE the call)
// is inlined here.
// Shape: the device number is fetched first (kept in edi); the inlined IsMainPlayer reads Number
// into a LOCAL before the call (ebx) -- written as `Number == ...` directly the lane reuses that load
// in the else arm and swaps ebx/edi (64 v 67); with the local the else arm re-reads Number like
// retail; the else arm is an if/else-if chain (`== device` stores 0, else Number) that VC7.1 folds into
// `sete cl ; dec ecx ; and ecx,eax` -- the `?:` spelling commutes it to `and eax,ecx`. Members (PlayerManager
// +0x10, Number +0x28) from engine/CPlayer.h; JoystickDeviceNumber sits at retail +0x228 (PDB +0x230:
// the retail PlayerModes list at +0x210 is 4 bytes, not 12), so the model is a local retail view.
struct CGameJoystickManager {
    static CGameJoystickManager& __fastcall Get();          // 0x0049BB90
    long GetMainPlayerJoystickDeviceNumber() const;         // 0x0049BB40
};
struct CPlayerManager {
    long GetMainPlayerNumber() const;                        // 0x00449700
};

struct CPlayer {
    unsigned char   _pad00[0x10];
    CPlayerManager* PlayerManager;                 // +0x10
    unsigned char   _pad14[0x14];
    long            Number;                        // +0x28
    unsigned char   _pad2c[0x228 - 0x2c];
    long            JoystickDeviceNumber;          // +0x228 (PDB +0x230)
    bool IsMainPlayer() const                      // out of line @ 0x00487520
    {
        long number = Number;
        return number == PlayerManager->GetMainPlayerNumber();
    }
    void InitJoystickDeviceNumber();
};

void CPlayer::InitJoystickDeviceNumber()
{
    long mainDevice = CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber();
    if (IsMainPlayer())
        JoystickDeviceNumber = mainDevice;
    else if (Number == mainDevice)
        JoystickDeviceNumber = 0;
    else
        JoystickDeviceNumber = Number;
}