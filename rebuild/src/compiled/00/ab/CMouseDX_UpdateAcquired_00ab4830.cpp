// CMouseDX::UpdateAcquired() -- give the device input focus (virtual slot 14, CMouse's SetHasInputFocus @ 0x00A673A0 in the base vtable 0x0129DBC4) when the application is active (0x009A57B0) and has input focus (0x009A4FB0, both on the system manager 0x009A4EC0), else take it away
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4830, length 64 bytes.
// The chunk label "CJoystickDX::UpdateAcquired" is propagated: slot 14 (+0x38) is CMouse::SetHasInputFocus, so this is the CMouseDX twin. Shape: two virtual call sites with `push 1` / `push 0`, each with its own epilogue, `ret`.
// Bindings (sweep 3): Ego_r spellings -- GFGetSystemManager is `__fastcall` returning class CSystemManager*
//   (?GFGetSystemManager@@YIPAVCSystemManager@@XZ, was YAPAU), and 0x009A4FB0 is CSystemManager::IsApplicationInputFocus
//   (?IsApplicationInputFocus@CSystemManager@@QAE_NXZ, was an invented HasInputFocus). A no-argument call is the same
//   under __cdecl and __fastcall. Same bytes.
class CSystemManager {
public:
    bool IsApplicationActive();                    // 0x009A57B0
    bool IsApplicationInputFocus();                // 0x009A4FB0 (`mov al,[ecx+0Ah]`: Ego_r ApplicationHasInputFocus +10)
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0 (Ego_r ?GFGetSystemManager@@YIPAVCSystemManager@@XZ)
struct CMouseDX {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void SetHasInputFocus(bool focus);     // slot 14 (+0x38)
    void UpdateAcquired();
};

void CMouseDX::UpdateAcquired()
{
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->IsApplicationInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
}