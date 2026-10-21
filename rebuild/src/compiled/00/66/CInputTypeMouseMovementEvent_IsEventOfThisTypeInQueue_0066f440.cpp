// CInputTypeMouseMovementEvent::IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) -- ask the input manager whether a joystick-queue event of type 13 is waiting for the player
// Leaf: ?IsEventOfThisTypeInQueue@CInputTypeMouseMovementEvent_Methods@@QBE_NW4EInputButtonEventType@@JAAM@Z
// Retail: Fable.exe @ 0x0066F440, 58 bytes, `ret 0Ch`; Ego_r.pdb ?IsEventOfThisTypeInQueue@CInputTypeMouseMovementEvent@@UBE_NW4EInputButtonEventType@@JAAM@Z
// (0x00676B90). Catalog label "CInputTypeMouseMovementEvent::..." names the wrong class for all but 0x0066F440.
// Body: a local CInputEvent (52 bytes; ctor 0x00A04410, out-of-line empty dtor 0x00A03B30) is passed to
// g_pInputManager (0x013B8388)->IsJoystickEventInQueue(13, 0, event, player) (0x009F4C60, PDB
// ?IsJoystickEventInQueue@CInputManager@@QBE_NW4EInputEventType@@JAAVCInputEvent@@J@Z); the result is held in bl across the dtor.
// The button event type and the float& are unused.
#include "engine/CInputTypeMouseMovementEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_MOVEMENT = 13 };
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
struct CInputTypeMouseMovementEvent_Methods : CInputTypeMouseMovementEvent {
    bool IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const;
};
bool CInputTypeMouseMovementEvent_Methods::IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const
{
    CInputEvent event;
    return g_pInputManager->IsJoystickEventInQueue(INPUT_EVENT_TYPE_MOUSE_MOVEMENT, 0, event, player);
}