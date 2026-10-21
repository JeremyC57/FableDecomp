// CInputTypeKeyboardKeyEvent::IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&) const -- is a matching keyboard event for this key queued? value = 1.0 if so
// Leaf: ?IsEventOfThisTypeInQueue@CInputTypeKeyboardKeyEvent@@UBE_NW4EInputButtonEventType@@JAAM@Z
// Retail: Fable.exe @ 0x0066F630, 116 bytes, `ret 0xc`. Local CInputEvent (ctor 0xA04410, dtor 0xA03B30, 0x34 bytes);
// GetEventTypeFromGeneralButtonType inlined here (1 -> 1, 2 -> 2, 3 -> 3, else 0), unlike the out-of-line mouse one
// 0x0066F060; CInputManager::IsKeyboardEventInQueue 0x009F50A0 (type, KeyboardKey, event) on g_pInputManager 0x013B8388.
// The player argument is unused. Sibling of the matched 0x0066F6D0 (mouse).
// Layout: Ego_r.pdb CInputTypeKeyboardKeyEvent (0x10): vptr, KeyboardKey +0x04, ControlDirection +0x08; CInputEvent 0x34 bytes.
// The 1/2/3 event-type values are not named (enum names not recovered).
enum EInputButtonEventType { EINPUTBUTTONEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EInputEventType { EINPUTEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EInputKey { EINPUTKEY_FORCE_DWORD = 0x7fffffff };

class CInputEvent {
public:
    CInputEvent();
    ~CInputEvent();
    char Data[0x34];
};

class CInputManager {
public:
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0
};
extern CInputManager* g_pInputManager;

class CInputTypeKeyboardKeyEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5
    EInputKey KeyboardKey;        // +0x04
    float ControlDirection[2];    // +0x08
private:
    EInputEventType GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const
    {
        switch (type) {
        case 1: return (EInputEventType)1;
        case 2: return (EInputEventType)2;
        case 3: return (EInputEventType)3;
        }
        return (EInputEventType)0;
    }
};

bool CInputTypeKeyboardKeyEvent::IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const
{
    CInputEvent event;
    if (g_pInputManager->IsKeyboardEventInQueue(GetEventTypeFromGeneralButtonType(type), KeyboardKey, event)) {
        value = 1.0f;
        return true;
    }
    return false;
}