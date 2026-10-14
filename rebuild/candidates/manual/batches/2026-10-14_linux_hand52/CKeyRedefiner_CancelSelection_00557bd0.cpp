// CKeyRedefiner::CancelSelection() -- when this redefiner is the selected one (0x013B8AC8) clear the selection, refresh the key text (0x00557A10), set the control base's state 0x21 (slot 4) and mode 0x19 (slot 3), and hand the control to the front-end manager (0x0041E5F2, slot 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557BD0, length 64 bytes.
// Callees: UpdateKeyText @ 0x00557A10, CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, no arguments), virtual slots of the control base at +4 and of the manager.
// Shape: `this` in esi compared against the global, then esi advanced to the control base (+4) for the three calls, `ret`.
struct CGuiControl_00557bd0 {                     // the second base (vptr at +4)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void SetMode(long mode);                // slot 3 (+0x0c)
    virtual void SetState(long state);              // slot 4 (+0x10)
};
struct CFrontEndManager {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void SetFocusControl(CGuiControl_00557bd0* control);   // slot 4 (+0x10)
};
CFrontEndManager* __fastcall GetInstance_0041e5f2();   // 0x0041E5F2
struct CKeyRedefinerRoot_00557bd0 {               // the first base (vptr at +0)
    virtual void v00();
};
struct CKeyRedefiner : CKeyRedefinerRoot_00557bd0, CGuiControl_00557bd0 {
    void UpdateKeyText();                          // 0x00557A10
    void CancelSelection();
};
extern CKeyRedefiner* g_pSelectedKeyRedefiner;     // 0x013B8AC8

void CKeyRedefiner::CancelSelection()
{
    if (g_pSelectedKeyRedefiner == this) {
        g_pSelectedKeyRedefiner = 0;
        UpdateKeyText();
        SetState(0x21);
        SetMode(0x19);
        GetInstance_0041e5f2()->SetFocusControl(this);
    }
}
