#include <cstdio>
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

static int g_ctor, g_dtor; static EInputEventType g_seenType; static long g_seenButton, g_seenPlayer; static CInputEvent* g_seenEvent; static bool g_inQueue;
CInputEvent::CInputEvent() { ++g_ctor; }
CInputEvent::~CInputEvent() { ++g_dtor; }
float CInputEvent::GetJoystickButtonPressureFraction() const { return this == g_seenEvent ? 0.75f : -1.0f; }
bool CInputManager::IsJoystickEventInQueue(EInputEventType type, long button, CInputEvent& event, long player) const
{ g_seenType = type; g_seenButton = button; g_seenEvent = &event; g_seenPlayer = player; return g_inQueue; }
static CInputManager g_mgr; CInputManager* g_pInputManager = &g_mgr;
void CInputTypeXboxPadButtonEvent::_v00() {} void CInputTypeXboxPadButtonEvent::_v01() {} void CInputTypeXboxPadButtonEvent::_v02() {}
void CInputTypeXboxPadButtonEvent::_v03() {} void CInputTypeXboxPadButtonEvent::_v04() {}
int main()
{
    CInputTypeXboxPadButtonEvent o; o.Button = (EXboxControllerButton)6; const CInputTypeXboxPadButtonEvent* p = &o;
    float v = 0.0f;
    g_inQueue = false;
    int ok = !p->IsEventOfThisTypeInQueue((EInputButtonEventType)2, 3, v) && v == 0.0f && g_seenType == 0x14 && g_seenButton == 6 && g_seenPlayer == 3 && g_ctor == 1 && g_dtor == 1;
    ok = ok && !p->IsEventOfThisTypeInQueue((EInputButtonEventType)9, 0, v) && g_seenType == 0;
    ok = ok && !p->IsEventOfThisTypeInQueue((EInputButtonEventType)3, 0, v) && g_seenType == 0x15;
    g_inQueue = true;
    ok = ok && p->IsEventOfThisTypeInQueue((EInputButtonEventType)1, 1, v) && v == 0.75f && g_seenType == 0x13 && g_seenPlayer == 1 && g_ctor == 4 && g_dtor == 4;
    printf(ok ? "0066f750_TEST PASS\n" : "0066f750_TEST FAIL\n");
    return ok ? 0 : 1;
}