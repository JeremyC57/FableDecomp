// CKeyboardDX::UpdateAcquired() -- give the keyboard input focus (virtual slot 3, SetHasInputFocus @ 0x00AB6060) when the application is active (0x009A57B0) and has input focus (0x009A4FB0, both on the system manager 0x009A4EC0), else take it away
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6100, length 57 bytes.
// The chunk label "CJoystickDX::UpdateAcquired" is propagated: slot 3 (+0xc) is CKeyboard::SetHasInputFocus (vtable 0x0129DBA8). Shape: two virtual call sites with `push 1` / `push 0`, `ret`.
struct CSystemManager {
    bool IsApplicationActive();                    // 0x009A57B0
    bool HasInputFocus();                          // 0x009A4FB0 (`mov al,[ecx+0Ah]`; labelled "CTCInventoryBase::GetCanItemBeQuickAssigned")
};
CSystemManager* __cdecl GFGetSystemManager();      // 0x009A4EC0
struct CKeyboardDX {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void SetHasInputFocus(bool focus);     // slot 3 (+0x0c)
    void UpdateAcquired();
};

void CKeyboardDX::UpdateAcquired()
{
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
}