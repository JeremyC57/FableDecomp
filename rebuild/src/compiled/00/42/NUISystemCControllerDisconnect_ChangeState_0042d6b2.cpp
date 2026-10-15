// NUISystem::CControllerDisconnect::ChangeState(unsigned long state) -- entering state 1 takes the input focus from the front end's current handler (remembered at +0x15c) and activates this screen's handler; state 0 hands the focus back; then the base ChangeState
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D6B2, 131 bytes. Vtable 0x0122FD64 slot 48.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`, no arguments) before every
//          manager call; manager vtable slots 7 (+0x1c, the current handler), 4 (+0x10, remove a handler
//          by pointer) and 3 (+0xc, add a handler by reference); the handler base's slot 6 (+0x18) on
//          this; CChangingStateComponent::ChangeState 0x0052CF40 (base call).
//
// Shape: the landed constructor (0x0041CBE4) model: CChangingStateComponent has a root polymorphic base
// at +0 and polymorphic bases at +4 (0x14 bytes, here the input-handler interface) and +0x18;
// CControllerDisconnect adds m_pPreviousHandler at +0x15c. The module is size-optimised (`#pragma
// optimize("s")`: `push dword ptr [mem]`, the `neg ; sbb ; and` null-tested conversion). Passing `*this`
// to the reference slot and calling the handler's own virtual on `this` need no null test; passing
// `this` as a handler POINTER does (`this ? this + 4 : 0`).
#pragma optimize("s", on)
namespace NUISystem {

struct CInputHandler_Retail {               // the polymorphic base at +4 (0x14 bytes)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void OnInputFocusGained();                  // slot 6 (+0x18)
    unsigned char _body[0x10];
};

struct CComponentBase3_Retail {             // base at +0x18
    virtual void w00();
};

struct CFrontEndManager_Calls {             // vtable-slot view of NUISystem::CFrontEndManager
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void AddInputHandler(CInputHandler_Retail& handler);      // slot 3 (+0xc)
    virtual void RemoveInputHandler(CInputHandler_Retail* handler);   // slot 4 (+0x10)
    virtual void v005();
    virtual void v006();
    virtual CInputHandler_Retail* GetCurrentInputHandler();           // slot 7 (+0x1c)
    static CFrontEndManager_Calls* __fastcall GetInstance();  // 0x0041E5F2
};

struct CComponentRoot_Retail {              // primary base: vptr only
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
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void ChangeState(unsigned long state);       // slot 48
};

struct CChangingStateComponent : CComponentRoot_Retail, CInputHandler_Retail, CComponentBase3_Retail {
    unsigned char _body[0x15c - 0x1c];      // +0x1c..+0x15c
    virtual void ChangeState(unsigned long state);   // 0x0052CF40
};

struct CControllerDisconnect : CChangingStateComponent {   // vtables 0x0122FD64 / 0x0122FD3C / 0x0122FD34
    CInputHandler_Retail* m_pPreviousHandler; // +0x15c
    virtual void ChangeState(unsigned long state);
};

void CControllerDisconnect::ChangeState(unsigned long state)
{
    if (state == 1) {
        m_pPreviousHandler = CFrontEndManager_Calls::GetInstance()->GetCurrentInputHandler();
        CFrontEndManager_Calls::GetInstance()->RemoveInputHandler(m_pPreviousHandler);
        CFrontEndManager_Calls::GetInstance()->AddInputHandler(*this);
        OnInputFocusGained();
    }
    if (state == 0) {
        CFrontEndManager_Calls::GetInstance()->RemoveInputHandler(this);
        CFrontEndManager_Calls::GetInstance()->AddInputHandler(*m_pPreviousHandler);
    }
    CChangingStateComponent::ChangeState(state);
}

}