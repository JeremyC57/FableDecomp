// CInputTypeXboxPadButtonEvent::IsEventOfThisType(const CInputEvent& event) -- does the event match this input type (joystick button types 19-21) and its Button
// Leaf: ?IsEventOfThisType@CInputTypeXboxPadButtonEvent_Methods@@QBE_NABVCInputEvent@@@Z
// Retail: Fable.exe @ 0x0066F250, 71 bytes, `ret 4`; Ego_r.pdb ?IsEventOfThisType@CInputTypeXboxPadButtonEvent@@UBE_NABVCInputEvent@@@Z (0x006769A0);
// catalog label names CInputTypeKeyboardKeyEvent for the class: wrong.
// Body: CInputEvent::GetType (0x00A03B40, out of line, called once per test) against 19, 20, 21; on a hit
// CInputEvent::GetJoystickButton (0x00A03B90) == Button (+0x04, `sub / neg / sbb / inc`), else false.
// Layout: engine/CInputTypeXboxPadButtonEvent.h (Button +0x04 as raw bytes, read in place).
#include "engine/CInputTypeXboxPadButtonEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_NONE };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    long GetJoystickButton() const;       // 0x00A03B90
};
struct CInputTypeXboxPadButtonEvent_Methods : CInputTypeXboxPadButtonEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeXboxPadButtonEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == 19 || event.GetType() == 20 || event.GetType() == 21)
        return event.GetJoystickButton() == *reinterpret_cast<const long*>(Button);
    return false;
}