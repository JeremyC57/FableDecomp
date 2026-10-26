// NUISystem::CZoomingComponent::ChangeState(unsigned long) -- on a non-zero state registers its input handler with the front end and gives it focus; on state 0 unregisters it; then the base ChangeState
// Retail 0x005469E0, 0x62 bytes. CZoomingComponent primary vtable 0x01247DC4 slot 48 (the pointer at 0x01247E84;
// same slot as the landed CControllerDisconnect::ChangeState 0x0042D6B2), so the class is verified from the vtable.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`, no arguments); manager vtable slots
// 2 (+0x08, add a handler by pointer) and 5 (+0x14, remove a handler by pointer), each passed `this ? this+4 : 0`
// (the input-handler base at +4, null-tested because it is converted as a pointer); the handler base's slot 6
// (+0x18) on this+4; CChangingStateComponent::ChangeState 0x0052CF40 (base call, both arms keep their own copy).
// Bases modelled as in the landed CControllerDisconnect / CZoomingComponent constructor (0x00546B00): root vptr +0,
// input handler +4 (0x14 bytes), third base +0x18, retail CChangingStateComponent 0x15C bytes.
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
    virtual void AddInputHandlerPtr(CInputHandler_Retail* handler);     // slot 2 (+0x08)
    virtual void v003();
    virtual void v004();
    virtual void RemoveInputHandlerPtr(CInputHandler_Retail* handler);  // slot 5 (+0x14)
    static CFrontEndManager_Calls* __fastcall GetInstance();  // 0x0041E5F2
};

struct CComponentRoot_Retail {              // primary base: vptr only (ChangeState is slot 48)
    virtual void v000();
};

class CChangingStateComponent : public CComponentRoot_Retail, public CInputHandler_Retail, public CComponentBase3_Retail { public:
    unsigned char _body[0x15c - 0x1c];      // +0x1c..+0x15c
    void ChangeState(unsigned long state);  // 0x0052CF40 (virtual slot 48 in retail)
};

class CZoomingComponent : public CChangingStateComponent { public:   // vtables 0x01247DC4 / 0x01247DA0 / 0x01247D98
    void ChangeState(unsigned long state);
};

void CZoomingComponent::ChangeState(unsigned long state)
{
    if (state != 0)
    {
        CFrontEndManager_Calls::GetInstance()->AddInputHandlerPtr(this);
        OnInputFocusGained();
    }
    else
        CFrontEndManager_Calls::GetInstance()->RemoveInputHandlerPtr(this);
    CChangingStateComponent::ChangeState(state);
}

}