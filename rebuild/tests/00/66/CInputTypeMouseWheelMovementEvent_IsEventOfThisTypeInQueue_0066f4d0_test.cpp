#include <cstdio>
#include "engine/CInputTypeMouseWheelMovementEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT = 14 };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_TYPE_NONE };
class CInputEvent {
public:
    CInputEvent();                                       // 0x00A04410
    ~CInputEvent();                                      // 0x00A03B30
    unsigned char _body[0x34];
};
class CInputManager {
public:
    bool IsJoystickEventInQueue(EInputEventType type, long index, CInputEvent& event, long player) const;  // 0x009F4C60
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
struct CInputTypeMouseWheelMovementEvent_Methods : CInputTypeMouseWheelMovementEvent {
    bool IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const;
};
bool CInputTypeMouseWheelMovementEvent_Methods::IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const
{
    CInputEvent event;
    return g_pInputManager->IsJoystickEventInQueue(INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT, 0, event, player);
}

static int g_ctor = 0, g_dtor = 0, g_type = -1; static long g_index = -1, g_player = -1; static const CInputEvent* g_ev = 0; static bool g_ret = false;
CInputEvent::CInputEvent() { ++g_ctor; }
CInputEvent::~CInputEvent() { ++g_dtor; }
bool CInputManager::IsJoystickEventInQueue(EInputEventType type, long index, CInputEvent& event, long player) const
{ g_type = type; g_index = index; g_player = player; g_ev = &event; return g_ret; }
static CInputManager g_mgr;
CInputManager* g_pInputManager = &g_mgr;
static CInputTypeMouseWheelMovementEvent_Methods g_obj;
int main()
{
    float f = 0.0f; bool ok = true;
    g_ret = true;
    if (!g_obj.IsEventOfThisTypeInQueue(INPUT_BUTTON_EVENT_TYPE_NONE, 3, f)) ok = false;
    if (g_type != 14 || g_index != 0 || g_player != 3 || g_ctor != 1 || g_dtor != 1 || !g_ev) ok = false;
    g_ret = false;
    if (g_obj.IsEventOfThisTypeInQueue(INPUT_BUTTON_EVENT_TYPE_NONE, 1, f) || g_player != 1 || g_dtor != 2) ok = false;
    if (ok) std::printf("0066f4d0_TEST PASS\n");
    return 0;
}