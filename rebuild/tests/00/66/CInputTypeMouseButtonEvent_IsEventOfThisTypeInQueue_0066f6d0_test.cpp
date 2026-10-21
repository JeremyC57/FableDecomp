#include <cstdio>
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

static int g_ctor, g_dtor; static EInputEventType g_seenType; static CInputEvent* g_seenEvent; static bool g_inQueue;
CInputEvent::CInputEvent() { ++g_ctor; }
CInputEvent::~CInputEvent() { ++g_dtor; }
bool CInputManager::IsMouseEventInQueue(EInputEventType type, CInputEvent& event) const { g_seenType = type; g_seenEvent = &event; return g_inQueue; }
static CInputManager g_mgr; CInputManager* g_pInputManager = &g_mgr;
void CInputTypeMouseButtonEvent::_v00() {} void CInputTypeMouseButtonEvent::_v01() {} void CInputTypeMouseButtonEvent::_v02() {}
void CInputTypeMouseButtonEvent::_v03() {} void CInputTypeMouseButtonEvent::_v04() {}
EInputEventType CInputTypeMouseButtonEvent::GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const { return (EInputEventType)(type * 10 + Button); }

int main()
{
    CInputTypeMouseButtonEvent o; o.Button = (EMouseButtonControl)3; const CInputTypeMouseButtonEvent* p = &o;
    float v = 0.0f;
    g_inQueue = false;
    int ok = !p->IsEventOfThisTypeInQueue((EInputButtonEventType)2, 0, v) && v == 0.0f && g_seenType == 23 && g_seenEvent && g_ctor == 1 && g_dtor == 1;
    g_inQueue = true;
    ok = ok && p->IsEventOfThisTypeInQueue((EInputButtonEventType)1, 5, v) && v == 1.0f && g_seenType == 13 && g_ctor == 2 && g_dtor == 2;
    printf(ok ? "0066f6d0_TEST PASS\n" : "0066f6d0_TEST FAIL\n");
    return ok ? 0 : 1;
}