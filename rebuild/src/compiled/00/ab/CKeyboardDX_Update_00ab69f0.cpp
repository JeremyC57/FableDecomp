// CKeyboardDX::Update() -- give or take input focus (slot 3) from the application's active/focus state (system manager 0x009A4EC0: 0x009A57B0, 0x009A4FB0), then read the key states (0x00AB5FF0) and build the events (0x00AB6240); always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB69F0, length 96 bytes.
// Vtable 0x0129EA60 slot 1. Callees: SetHasInputFocus (slot 3), UpdateKeyStates @ 0x00AB5FF0, UpdateKeyEvents @ 0x00AB6240. Shape: the two focus arms each carry the tail (the inlined UpdateAcquired, 0x00AB6100, duplicated with the calls), `mov al,1`, `ret`.
class CSystemManager { public:
    bool IsApplicationActive();                    // 0x009A57B0
    bool IsApplicationInputFocus();                          // 0x009A4FB0 (`mov al,[ecx+0Ah]`; labelled "CTCInventoryBase::GetCanItemBeQuickAssigned") ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ (Ego_r.pdb + FableWin.pdb; NF197: was HasInputFocus, in neither PDB)
};
CSystemManager* __fastcall GFGetSystemManager();      // 0x009A4EC0
struct CKeyboardDX {
    virtual void v00();
    virtual bool Update();                         // slot 1
    virtual void v02();
    virtual void SetHasInputFocus(bool focus);     // slot 3 (+0x0c)
protected:   // NF374: Ego_r + FableWin ?UpdateKeyStates@CKeyboardDX@@IAE_NXZ / ?UpdateKeyEvents@CKeyboardDX@@IAE_NXZ (was void UpdateEvents)
    bool UpdateKeyStates();                        // 0x00AB5FF0
    bool UpdateKeyEvents();                        // 0x00AB6240
public:
    void UpdateAcquired()                          // 0x00AB6100 (the out-of-line copy), inlined here
    {
        if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->IsApplicationInputFocus())
            SetHasInputFocus(true);
        else
            SetHasInputFocus(false);
    }
};

bool CKeyboardDX::Update()
{
    UpdateAcquired();
    UpdateKeyStates();
    UpdateKeyEvents();
    return true;
}