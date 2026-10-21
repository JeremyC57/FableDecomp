// CInputTypeMouseButtonEvent::IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&) const -- is a matching mouse event queued? value = 1.0 if so
// Leaf: ?IsEventOfThisTypeInQueue@CInputTypeMouseButtonEvent@@UBE_NW4EInputButtonEventType@@JAAM@Z
// Retail: Fable.exe @ 0x0066F6D0, 88 bytes, `ret 0xc`; vtable 0x012318C8 slot 5. Local CInputEvent (ctor 0xA04410, dtor 0xA03B30, 0x34 bytes);
// GetEventTypeFromGeneralButtonType 0x0066F060 (`ret 4`, one arg); CInputManager::IsMouseEventInQueue 0x009F4C00 on g_pInputManager 0x013B8388.
// The player argument is unused.
// Layout: Ego_r.pdb CInputTypeMouseButtonEvent (0x10): vptr, Button +0x04, ControlDirection +0x08; CInputEvent 0x34 bytes.
enum EInputButtonEventType { EINPUTBUTTONEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EInputEventType { EINPUTEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EMouseButtonControl { EMOUSEBUTTONCONTROL_FORCE_DWORD = 0x7fffffff };

class CInputEvent {
public:
    CInputEvent();
    ~CInputEvent();
    char Data[0x34];
};

class CInputManager {
public:
    bool IsMouseEventInQueue(EInputEventType type, CInputEvent& event) const;
};
extern CInputManager* g_pInputManager;

class CInputTypeMouseButtonEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5
    EMouseButtonControl Button;   // +0x04
    float ControlDirection[2];    // +0x08
private:
    EInputEventType GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const;
};

bool CInputTypeMouseButtonEvent::IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const
{
    CInputEvent event;
    if (g_pInputManager->IsMouseEventInQueue(GetEventTypeFromGeneralButtonType(type), event)) {
        value = 1.0f;
        return true;
    }
    return false;
}