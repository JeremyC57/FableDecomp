// CInputTypeXboxPadButtonEvent::IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&) const -- is a matching pad-button event queued for this player? value = its pressure fraction if so
// Leaf: ?IsEventOfThisTypeInQueue@CInputTypeXboxPadButtonEvent@@UBE_NW4EInputButtonEventType@@JAAM@Z
// Retail: Fable.exe @ 0x0066F750, 125 bytes, `ret 0xc`; vtable 0x012317A8 slot 5 (slot 4 is the matched 0x0066F730). Local
// CInputEvent (ctor 0xA04410, dtor 0xA03B30, 0x34 bytes); GetEventTypeFromGeneralButtonType inlined (1 -> 0x13, 2 -> 0x14,
// 3 -> 0x15, else 0); CInputManager::IsJoystickEventInQueue 0x009F4C60 (type, Button, event, player) on g_pInputManager
// 0x013B8388 (Ego_r.pdb ?IsJoystickEventInQueue@CInputManager@@QBE_NW4EInputEventType@@JAAVCInputEvent@@J@Z); value =
// event.GetJoystickButtonPressureFraction() (0x00A03BA0, labelled CMap::DrawGetSeaLevel -- a folded float getter).
// Layout: Ego_r.pdb CInputTypeXboxPadButtonEvent (8 bytes): vptr, Button +0x04; CInputEvent 0x34 bytes.
enum EInputButtonEventType { EINPUTBUTTONEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EInputEventType { EINPUTEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EXboxControllerButton { EXBOXCONTROLLERBUTTON_FORCE_DWORD = 0x7fffffff };

class CInputEvent {
public:
    CInputEvent();
    ~CInputEvent();
    float GetJoystickButtonPressureFraction() const;     // 0x00A03BA0
    char Data[0x34];
};

class CInputManager {
public:
    bool IsJoystickEventInQueue(EInputEventType type, long button, CInputEvent& event, long player) const;   // 0x009F4C60
};
extern CInputManager* g_pInputManager;

class CInputTypeXboxPadButtonEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5
    EXboxControllerButton Button; // +0x04
private:
    EInputEventType GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const
    {
        switch (type) {
        case 1: return (EInputEventType)0x13;
        case 2: return (EInputEventType)0x14;
        case 3: return (EInputEventType)0x15;
        }
        return (EInputEventType)0;
    }
};

bool CInputTypeXboxPadButtonEvent::IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const
{
    CInputEvent event;
    if (g_pInputManager->IsJoystickEventInQueue(GetEventTypeFromGeneralButtonType(type), Button, event, player)) {
        value = event.GetJoystickButtonPressureFraction();
        return true;
    }
    return false;
}