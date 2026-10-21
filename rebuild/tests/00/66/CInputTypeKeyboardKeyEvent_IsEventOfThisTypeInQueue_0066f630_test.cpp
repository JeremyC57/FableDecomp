#include <cstdio>
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

static int g_ctor, g_dtor; static EInputEventType g_seenType; static EInputKey g_seenKey; static CInputEvent* g_seenEvent; static bool g_inQueue;
CInputEvent::CInputEvent() { ++g_ctor; }
CInputEvent::~CInputEvent() { ++g_dtor; }
bool CInputManager::IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const { g_seenType = type; g_seenKey = key; g_seenEvent = &event; return g_inQueue; }
static CInputManager g_mgr; CInputManager* g_pInputManager = &g_mgr;
void CInputTypeKeyboardKeyEvent::_v00() {} void CInputTypeKeyboardKeyEvent::_v01() {} void CInputTypeKeyboardKeyEvent::_v02() {}
void CInputTypeKeyboardKeyEvent::_v03() {} void CInputTypeKeyboardKeyEvent::_v04() {}
int main()
{
    CInputTypeKeyboardKeyEvent o; o.KeyboardKey = (EInputKey)0x41; const CInputTypeKeyboardKeyEvent* p = &o;
    float v = 0.0f;
    g_inQueue = false;
    int ok = !p->IsEventOfThisTypeInQueue((EInputButtonEventType)2, 0, v) && v == 0.0f && g_seenType == 2 && g_seenKey == 0x41 && g_seenEvent && g_ctor == 1 && g_dtor == 1;
    ok = ok && !p->IsEventOfThisTypeInQueue((EInputButtonEventType)7, 0, v) && g_seenType == 0;
    ok = ok && !p->IsEventOfThisTypeInQueue((EInputButtonEventType)0, 0, v) && g_seenType == 0;
    ok = ok && !p->IsEventOfThisTypeInQueue((EInputButtonEventType)3, 0, v) && g_seenType == 3;
    g_inQueue = true;
    ok = ok && p->IsEventOfThisTypeInQueue((EInputButtonEventType)1, 5, v) && v == 1.0f && g_seenType == 1 && g_ctor == 5 && g_dtor == 5;
    printf(ok ? "0066f630_TEST PASS\n" : "0066f630_TEST FAIL\n");
    return ok ? 0 : 1;
}